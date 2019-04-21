#include"init.h"

void *writer(int *buffer){
	sem_wait(&db);
	(*buffer)++;
    printf ("write ::%d\n", *buffer);
    sem_post(&db);
    pthread_exit(0);
}
