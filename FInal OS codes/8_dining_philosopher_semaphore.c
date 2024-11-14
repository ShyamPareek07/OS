#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

sem_t forks[NUM_PHILOSOPHERS];    // Semaphores representing forks
pthread_t philosophers[NUM_PHILOSOPHERS];

void* philosopher(void* num) {
    int id = *(int*)num;

    while (1) {
        // Thinking
        printf("Philosopher %d is thinking.\n", id);
        //sleep(rand() % 3 + 1);  // Simulate thinking time
        for(int i = 0; i < 1000000000; i++);
        // Hungry, try to pick up forks
        printf("Philosopher %d is hungry.\n", id);

        // Pick up the left fork
        sem_wait(&forks[id]);
        printf("Philosopher %d picked up fork %d (left).\n", id, id);

        // Pick up the right fork
        sem_wait(&forks[(id + 1) % NUM_PHILOSOPHERS]);
        printf("Philosopher %d picked up fork %d (right).\n", id, (id + 1) % NUM_PHILOSOPHERS);

        // Eating
        printf("Philosopher %d is eating.\n", id);
        //sleep(rand() % 3 + 1);  // Simulate eating time

        for(int i = 0; i < 1000000000; i++);
        // Put down the forks
        sem_post(&forks[id]);
        printf("Philosopher %d put down fork %d (left).\n", id, id);

        sem_post(&forks[(id + 1) % NUM_PHILOSOPHERS]);
        printf("Philosopher %d put down fork %d (right).\n", id, (id + 1) % NUM_PHILOSOPHERS);
    }
}

int main() {
    int i;
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize semaphores (one for each fork)
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // Create philosopher threads
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        if (pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]) != 0) {
            perror("Failed to create philosopher thread");
            exit(1);
        }
    }

    // Wait for all philosopher threads to complete (this program runs indefinitely)
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}

