#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>

//N is the sum of buffer pool
#define N 10
typedef struct share_status{
	int in;
	int out;
	int buffer[N];
}share_status;
union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};
key_t sem_key = 1234
key_t shm_key = 1234
int set_semaphore(int order, int value);
int del_semaphore();
int get_semaphore(int order);
int semaphore_p();
int semaphore_v();

int main(){
	int i;
	char answer;
	void *share_buf = (void *)0;
	share_status *shared_status;

	srand((unsigned int)getpid());

	printf("Are you a producer ?Please input y or n\n");
	scanf("%c",&answer);
	if(answer == 'y'){
		while(1){
			//p the empty
			if(!semaphore_p(1)){
				printf("producer's p function failed\n");
				exit(EXIT_FAILURE);
			}
			//p the mutex
			if(!semaphore_p(0)){
				printf("producer's p function failed\n");
				exit(EXIT_FAILURE);
			}
			//the critical area
			shared_status->in = shared_status->in % N;
			shared_status->buffer[shared_status->in] = 1;
			printf("Process %d put a resource and the sum is %d", getpid(),get_semaphore(2) + 1);
			printf("the buffer pool condition:");
			for(i = 0; i < N; i ++){
				printf("%d",shared_status->buffer[i]);
			}
			printf("\n");
			shared_status->in ++;

			sleep(rand() % 3);
			//the critical area end
			//v the mutex
			if(!semaphore_v(0)){
				printf("producer's v function failed\n");
				exit(EXIT_FAILURE);
			}
			//v the full
			if(!semaphore_v(2)){
				printf("producer's v function failed\n");
				exit(EXIT_FAILURE);
			}
		}
	}else{
		while(1){
			if(!semaphore_p(2)){
				printf("consumer's p function failed\n");
				exit(EXIT_FAILURE);
			}
			if(!semaphore_p(0)){
				printf("consumer's p function failed\n");
				exit(EXIT_FAILURE);
			}
			shared_status->out = shared_status->out % N;
			shared_status->buffer[shared_status->out] = 0;
			printf("Process %d get a resource and the sum is%d ", getpid(),get_semaphore(2) - 1);
			printf("the buffer pool condition:");
			for(i = 0; i < N; i ++){
				printf("%d",shared_status->buffer[i]);
			}
			printf("\n");
			shared_status->out ++;

			sleep(rand() % 3);
			if(!semaphore_v(0)){
				printf("consumer's v function failed\n");
				exit(EXIT_FAILURE);
			}
			if(!semaphore_v(1)){
				printf("consumer's v function failed\n");
				exit(EXIT_FAILURE);
			}
		}
	
	}
	del_semaphore();
	shmdt(shared_status);
	exit(EXIT_SUCCESS);
}