#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;   // Counter to keep track of items in the buffer
int in = 0, out = 0;

pthread_mutex_t mutex;      // Mutex for critical section
pthread_cond_t not_full;    // Condition variable for buffer not full
pthread_cond_t not_empty;   // Condition variable for buffer not empty

void *producer(void *arg) {
    int item;
    for (int i = 0; i < 10; i++) {  // Producing 10 items
        item = rand() % 100;  // Produce an item (a random number)

        pthread_mutex_lock(&mutex);  // Lock mutex before entering critical section

        while (count == BUFFER_SIZE) {  // Wait if buffer is full
            pthread_cond_wait(&not_full, &mutex);
        }

        // Add item to buffer
        buffer[in] = item;
        printf("Producer produced: %d at position %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        count++;  // Increment buffer count

        pthread_cond_signal(&not_empty);  // Signal that buffer is not empty
        pthread_mutex_unlock(&mutex);     // Unlock mutex

        sleep(1);  // Sleep for demonstration
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int item;
    for (int i = 0; i < 10; i++) {  // Consuming 10 items
        pthread_mutex_lock(&mutex);  // Lock mutex before entering critical section

        while (count == 0) {  // Wait if buffer is empty
            pthread_cond_wait(&not_empty, &mutex);
        }

        // Remove item from buffer
        item = buffer[out];
        printf("Consumer consumed: %d from position %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        count--;