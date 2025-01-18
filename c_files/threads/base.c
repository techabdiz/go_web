#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>


void *thread(void *vargp);

volatile long cnt = 0;
sem_t mutex;

int main () { 
    sem_init(&mutex, 0, 2);
    int niters = 100000;
    int num_threads = 2; 
    pthread_t arr[num_threads];
    for( int i = 0; i < num_threads; i ++ ) {
        pthread_t t;
        pthread_create(&t, NULL, thread, &niters);
        arr[i] = t;
    }  

    for( int i = 0; i < num_threads; i ++ ) {
            pthread_join(arr[i], NULL);
    }

    /*pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    pthread_create(&t1, NULL, thread, &niters);
    pthread_create(&t2, NULL, thread, &niters);
    pthread_create(&t3, NULL, thread, &niters);
    pthread_create(&t4, NULL, thread, &niters);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);*/
    printf("return val: %d\n", cnt);
    return 0;
}

void* thread(void *vargp) {
    
    for (int i = 0; i < *(int*)vargp; i++) { 
        sem_wait(&mutex);
        cnt++;
        sem_post(&mutex);
    }
    return NULL;
}