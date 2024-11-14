#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t resource_access = PTHREAD_MUTEX_INITIALIZER; // Mutex for resource access
pthread_mutex_t read_count_access = PTHREAD_MUTEX_INITIALIZER; // Mutex for read count access
int read_count = 0; // Number of readers currently accessing the resource

void *reader(void *arg) {
    int reader_id = *(int *)arg;

    while (1) {
        // Reader wants to read, acquire lock on read count
        pthread_mutex_lock(&read_count_access);
        read_count++;
        if (read_count == 1) {
            // First reader locks the resource
            pthread_mutex_lock(&resource_access);
        }
        pthread_mutex_unlock(&read_count_access);

        // Reading section (critical section for reading)
        printf("Reader %d is reading\n", reader_id);
        //sleep(1); // Simulating reading time
        for (int i = 0; i < 100000000; i++);
        // Reader finished reading
        pthread_mutex_lock(&read_count_access);
        read_count--;
        if (read_count == 0) {
            // Last reader unlocks the resource
            pthread_mutex_unlock(&resource_access);
        }
        pthread_mutex_unlock(&read_count_access);

        // Simulating time between read operations
        // sleep(rand() % 3);
        for(int i = 0; i < 1000000000; i++);
    }
}

void *writer(void *arg) {
    int writer_id = *(int *)arg;

    while (1) {
        // Writer wants to write, lock the resource for exclusive access
        pthread_mutex_lock(&resource_access);

        // Writing section (critical section for writing)
        printf("Writer %d is writing\n", writer_id);
        //sleep(1); // Simulating writing time
        for (int i = 0; i < 100000000; i++);

        // Writer finished writing, unlock the resource
        pthread_mutex_unlock(&resource_access);

        // Simulating time between write operations
        //sleep(rand() % 3);
        for (int i = 0; i < 100000000; i++);
    }
}

int main() {
    int i;
    pthread_t readers[5], writers[5];
    int reader_ids[5], writer_ids[5];

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

    return 0;
}