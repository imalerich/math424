#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*
 * Implements the producer/consumer problem
 * generalized to 2k threads.
 * Even threads are consumers, 
 * odd threads are producers.
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
	if (thread_count % 2 == 1) {
		printf("ERROR - Thread count must be even.\n");
		exit(EXIT_FAILURE);
	}
	int num_consumers = thread_count / 2;

	/* --- Setup --- */
	Data data;
	data.locks = malloc(num_consumers * sizeof(pthread_mutex_t));
	data.messages = malloc(num_consumers * sizeof(char *));
	for (int i=0; i<num_consumers; i++) {
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
	for (int i=0; i<num_consumers; i++) {
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

	// this is the message queue we will either be
	// writing from or reading to, we will also
	// lock/unlock the corresponding mutex
	int queue_id = my_rank/2;

	while (1) {
		pthread_mutex_lock(&locks[queue_id]);

		if (my_rank % 2 == 0) {
			if (messages[queue_id] != NULL) {
				// print message
				printf("Consumer [%d] Received: %s\n", my_rank, messages[queue_id]);
				pthread_mutex_unlock(&locks[queue_id]);
				free(messages[queue_id]);
				break;
			}
		} else {
			// create the message
			char * out = malloc(sizeof(char) * 256);
			sprintf(out, "Hello from Producer [%d]!", my_rank);
	
			// and send it to the consumer
			messages[queue_id] = out;
			pthread_mutex_unlock(&locks[queue_id]);
			break;
		}

		pthread_mutex_unlock(&locks[queue_id]);
	}
}
