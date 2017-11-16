#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*
 * Implements the producer/consumer problem
 * for n threads, where each thread is both
 * a producer and a consumer.
 * Each thread will send a message to the 'next'
 * thread (my_rank+1) % n.
 * (page 223)
 */

typedef struct Data {
	pthread_mutex_t * locks;
	char ** messages;
} Data;

typedef struct Memory {
	int id;
	int thread_count;
	Data * data;
} Memory;

/**
 * Main execution method for our threads.
 */
void * thread_main(void * p_memory);

int main(int argc, char ** argv) {
	if (argc == 1) {
		printf("Please include the thread_count in program arguments.\n");
		exit(EXIT_FAILURE);
	}
	int thread_count = strtol(argv[1], NULL, 10);
	if (thread_count < 2) {
		printf("ERROR - At least 2 threads required.\n");
		exit(EXIT_FAILURE);
	}

	/* --- Setup --- */
	Data data;
	data.locks = malloc(thread_count * sizeof(pthread_mutex_t));
	data.messages = malloc(thread_count * sizeof(char *));
	for (int i=0; i<thread_count; i++) {
		pthread_mutex_init(&data.locks[i], NULL);
		data.messages[i] = NULL;
	}

	/* --- Spawn the producer/consumer threads. --- */
	pthread_t * threads = malloc(sizeof(pthread_t) * thread_count);
	Memory * p_mem = malloc(sizeof(Memory) * thread_count);

	for (int i=0; i<thread_count; i++) {
		Memory m = {i, thread_count, &data};
		p_mem[i] = m;
		pthread_create(&threads[i], NULL, thread_main, (void *)&p_mem[i]);
	}

	for (int i=0; i<thread_count; i++) {
		pthread_join(threads[i], NULL);
	}

	/* --- Cleanup --- */
	for (int i=0; i<thread_count; i++) {
		pthread_mutex_destroy(&data.locks[i]);
	}

	free(data.messages);
	free(data.locks);
	free(threads);
	free(p_mem);
}

void * thread_main(void * p_memory) {
	Memory * mem = (Memory *)p_memory;
	int my_rank = mem->id;
	int thread_count = mem->thread_count;

	Data * data = mem->data;
	pthread_mutex_t * locks = data->locks;
	char ** messages = data->messages;

	int send_id = (my_rank + thread_count + 1) % thread_count;
	int recv_id = (my_rank + thread_count - 1) % thread_count;

	/* ---------------------------------------------------
	 * everyone will send first and then receive
	 * only one thread is sending to a particular consumer
	 * so we don't actually need to loop for the producer
	 * --------------------------------------------------- */
	
	pthread_mutex_lock(&locks[send_id]);
	// create the message
	char * out = malloc(sizeof(char) * 256);
	sprintf(out, "Hello from Producer [%d]!", my_rank);
	// and send it to the consumer
	messages[send_id] = out;
	pthread_mutex_unlock(&locks[send_id]);

	/* ------------------------------
	 * the consumer will have to loop
	 * in case the conditional fails
	 * ------------------------------ */
	while (1) {
		pthread_mutex_lock(&locks[recv_id]);
		if (messages[recv_id] != NULL) {
			// print message
			printf("Consumer [%d] Received: %s\n", my_rank, messages[recv_id]);
			pthread_mutex_unlock(&locks[recv_id]);
			free(messages[recv_id]);
			break;
		}

		pthread_mutex_unlock(&locks[recv_id]);
	}
}
