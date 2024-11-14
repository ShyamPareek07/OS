#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty;  // Semaphore to track empty slots in the buffer
sem_t full;   // Semaphore to track filled slots in the buffer
pthread_mutex_t mutex;  // Mutex to protect critical section

void *producer(void *arg) {
    int item;
    for (int i = 0; i < 10; i++) {  // Producing 10 items
        item = rand() % 100;  // Produce an item (a random number)
        sem_wait(&empty);  // Wait if buffer is full
        pthread_mutex_lock(&mutex);  // Enter critical section

        buffer[in] = item;  // Add item to buffer
        printf("Producer produced: %d at position %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);  // Leave critical section
        sem_post(&full);  // Signal that buffer has a new item

        sleep(1);  // Sleep for demonstration
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int item;
    for (int i = 0; i < 10; i++) {  // Consuming 10 items
        sem_wait(&full);  // Wait if buffer is empty
        pthread_mutex_lock(&mutex);  // Enter critical section

        item = buffer[out];  // Remove item from buffer
        printf("Consumer consumed: %d from position %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);  // Leave critical section
        sem_post(&empty);  // Signal that buffer has an empty slot

        sleep(1);  // Sleep for demonstration
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty, 0, BUFFER_SIZE);  // Initialize empty to BUFFER_SIZE
    sem_init(&full, 0, 0);  // Initialize full to 0
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for both threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Destroy semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}