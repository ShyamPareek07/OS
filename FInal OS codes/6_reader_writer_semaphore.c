#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t resource_access;     // Semaphore for resource access control
sem_t read_count_access;   // Semaphore for read count access control
int read_count = 0;        // Number of readers currently accessing the resource

void *reader(void *arg) {
    int reader_id = *(int *)arg;

    while (1) {
        // Reader wants to read, acquire lock on read count
        sem_wait(&read_count_access);
        read_count++;
        if (read_count == 1) {
            // First reader locks the resource
            sem_wait(&resource_access);
        }
        sem_post(&read_count_access);

        // Reading section (critical section for reading)
        printf("Reader %d is reading\n", reader_id);
        //sleep(1); // Simulating reading time
        for(int i = 0; i < 1000000000; i++);

        // Reader finished reading
        sem_wait(&read_count_access);
        read_count--;
        if (read_count == 0) {
            // Last reader unlocks the resource
            sem_post(&resource_access);
        }
        sem_post(&read_count_access);

        // Simulating time between read operations
        // sleep(rand() % 3);
        for(int i = 0; i < 1000000000; i++);
    }
}

void *writer(void *arg) {
    int writer_id = *(int *)arg;

    while (1) {
        // Writer wants to write, lock the resource for exclusive access
        sem_wait(&resource_access);

        // Writing section (critical section for writing)
        printf("Writer %d is writing\n", writer_id);
        // sleep(1); // Simulating writing time
        for(int i = 0; i < 1000000000; i++);

        // Writer finished writing, unlock the resource
        sem_post(&resource_access);

        // Simulating time between write operations
        //sleep(rand() % 3);
        for(int i = 0; i < 1000000000; i++);
    }
}

int main() {
    int i;
    pthread_t readers[5], writers[5];
    int reader_ids[5], writer_ids[5];

    // Initialize semaphores
    sem_init(&resource_access, 0, 1);     // Initialize resource access semaphore to 1
    sem_init(&read_count_access, 0, 1);   // Initialize read count access semaphore to 1

    // Create reader threads
    for (i = 0; i < 5; i++) {
        reader_ids[i] = i + 1;
        if (pthread_create(&readers[i], NULL, reader, &reader_ids[i]) != 0) {
            perror("Failed to create reader thread");
            exit(1);
        }
    }

    // Create writer threads
    for (i = 0; i < 5; i++) {
        writer_ids[i] = i + 1;
        if (pthread_create(&writers[i], NULL, writer, &writer_ids[i]) != 0) {
            perror("Failed to create writer thread");
            exit(1);
        }
    }

    // Wait for all threads to finish
    for (i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
        pthread_join(writers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&resource_access);
    sem_destroy(&read_count_access);

    return 0;
}
