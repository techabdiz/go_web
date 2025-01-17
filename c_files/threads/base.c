#include <stdio.h>
#include <pthread.h>

void *thread(void *vargp);

int main ( ) { 
    pthread_t pt;
    pthread_create(&pt, NULL, thread, "1");
    pthread_create(&pt, NULL, thread, "2");
    pthread_create(&pt, NULL, thread, "3");
    pthread_create(&pt, NULL, thread, "4");
    pthread_join(pt, NULL);
    printf("main exit... !\n");
    return 0;
}

void* thread(void *vargp) { 
    printf("Hello from thread %s\n", vargp);
    return NULL;
}