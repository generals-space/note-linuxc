#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

//N is the sum of buffer pool
#define N 10

typedef struct {
	int in;
	int out;
	int buffer[N];
} share_status;

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

key_t sem_key, shm_key;

int set_semaphore(int order, int value);
int del_semaphore();
int get_semaphore(int order);
int semaphore_p();
int semaphore_v();
