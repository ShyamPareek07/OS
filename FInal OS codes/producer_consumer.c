//Name : Apoorva Pendse
//PRN : 12211435
//TY IT C
//Roll : 15
#include<semaphore.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>


int buffer[10];
int count = 0; 
sem_t empty;
sem_t fill;
pthread_mutex_t mutex;

void* produce(){
    while(1){
        sem_wait(&empty); //wait for an empty slot

        pthread_mutex_lock(&mutex);
            int item = rand() % 100;
            if(count<10){
                buffer[count++] = item;
                printf("produced item %d at index %d\n",item, count-1);
            }else{
                printf("skipping production since buffer is full\n");
            }
        pthread_mutex_unlock(&mutex);
        sem_post(&fill); //increment the number of filled slots
    }
}
void* consume(){

    while(1){
        sem_wait(&fill); //wait for filled slot

        pthread_mutex_lock(&mutex);
        if(count > 0){
            int removed = buffer[count-1];
            printf("removed item %d at index %d\n",removed, count-1);
            count--;
        }else{
            printf("skipping consumption since buffer is full\n");
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); //signal the empty slots after consumption
    }
}
int main(){
    pthread_t producer;
    pthread_t consumer;

    pthread_mutex_init(&mutex,NULL);
    sem_init(&empty,0,10); //initial empty slots are 10
    sem_init(&fill, 0, 0); //initial filled slots are 0;

    pthread_create(&producer, NULL, produce , NULL);
    pthread_create(&producer, NULL, consume , NULL);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    pthread_mutex_destroy(&mutex);

    
}