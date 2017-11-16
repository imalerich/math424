#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*
 * Implements the producer/consumer problem
 * using only two threads. 
 * A producer thread, and a consumer thread.
 * (page 223)
 */

#define PRODUCER 0
#define CONSUMER 1

typedef struct Data {
	pthread_mutex_t * locks;
	char ** messages;
} Data;

typedef struct Memory {
	int id;
	Data data;
} Memory;

/**
 * Main execution method for our threads.
 */
void * thread_main(void * p_memory);

int main(int argc, int argv) {
	/* --- Setup --- */
	Data data;
	data.locks = malloc(sizeof(pthread_mutex_t));
	data.messages = malloc(sizeof(char *));
	pthread_mutex_init(&data.locks[0], NULL);
	data.messages[0] = NULL;

	/* --- Spawn the producer/consumer threads. --- */
	Memory cmem = {CONSUMER, data};
	Memory pmem = {PRODUCER, data};

	pthread_t consumer, producer;
	pthread_create(&consumer, NULL, thread_main, (void *)&cmem);
	pthread_create(&producer, NULL, thread_main, (void *)&pmem);

	pthread_join(consumer, NULL);
	pthread_join(producer, NULL);

	/* --- Cleanup --- */
	pthread_mutex_destroy(&data.locks[0]);
	free(data.messages);
}

void * thread_main(void * p_memory) {
	Memory * mem = (Memory *)p_memory;
	int my_rank = mem->id;

	Data data = mem->data;
	pthread_mutex_t * locks = data.locks;
	char ** messages = data.messages;

	while (1) {
		pthread_mutex_lock(&locks[0]);

		if (my_rank == CONSUMER) {
			if (messages[0] != NULL) {
				// print message
				printf("Consumer Received: %s\n", messages[0]);
				pthread_mutex_unlock(&locks[0]);
				free(messages[0]);
				break;
			}
		} else {
			// create message
			const char * msg = "Hello from the Producer!";
			char * out = malloc(sizeof(char) * (strlen(msg)+1));
			strcpy(out, msg);
			messages[0] = out;
			pthread_mutex_unlock(&locks[0]);
			break;
		}

		pthread_mutex_unlock(&locks[0]);
	}
}
