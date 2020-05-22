#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

static pthread_mutex_t mtx_first = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mtx_second = PTHREAD_MUTEX_INITIALIZER;
int i=0;


static void function1() {
    printf("first function\n");
    pthread_mutex_lock(&mtx_first);
    do{
        i = rand();
        printf("i = %d", i);
    }while(1);
    // pthread_mutex_lock(&mtx_second);
    // pthread_mutex_unlock(&mtx_second);
    // pthread_mutex_unlock(&mtx_first);
}

static void function2() {
    printf("second function\n");
    // pthread_mutex_lock(&mtx_second);
    pthread_mutex_lock(&mtx_first);
    do{
        i = rand();
        printf("i = %d", i);
    }while(1);
    pthread_mutex_unlock(&mtx_first);
    // pthread_mutex_unlock(&mtx_second);
}

int main(int argc, char* argv[]) {
    pthread_t t1, t2;

    if (pthread_create(&t1, NULL, (void*)function1, NULL)!=0) {
        printf("Error: cannot create first thread\n");
        return -1;
    }
    if (pthread_create(&t2, NULL, (void*)function2, NULL)!=0) {
        printf("Error: cannot create second thread\n");
        return -1;
    }

    if (pthread_join(t1, 0) != 0) {
        printf("Error: cannot join first thread\n");
        return -1;
    }

    if (pthread_join(t2, 0) != 0) {
        printf("Error: cannot join second thread\n");
        return -1;
    }
}

//gcc deadlock.c -lpthread -o deadlock.o && ./deadlock.o