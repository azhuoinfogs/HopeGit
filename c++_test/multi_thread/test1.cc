#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
 
int count = 0;
 
void *decrement(void *arg) {
    printf("in derement.\n");
    pthread_mutex_lock(&mutex);
    if (count == 0)
        pthread_cond_wait(&cond, &mutex);
    count--;
    printf("----decrement:%d.\n", count);
    printf("out decrement.\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}
 
void *increment(void *arg) {
    printf("in increment.\n");
    pthread_mutex_lock(&mutex);
    count++;
    printf("----increment:%d.\n", count);
    if (count != 0)
        pthread_cond_signal(&cond);
    printf("out increment.\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}
 
int main(int argc, char *argv[]) {
    pthread_t tid_in, tid_de;
    pthread_create(&tid_de, NULL, decrement, NULL);
    sleep(2);
    pthread_create(&tid_in, NULL, increment, NULL);
    sleep(5);
    pthread_join(tid_de, NULL);
    pthread_join(tid_in, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}