#include "sema_common.h"

sem_key = 1234;
shm_key = 1234;

int set_semaphore(int order, int value){
	int sem_id = semget(sem_key, 3, 0666 | IPC_CREAT);

	union semun sem_union;
	sem_union.val = value;
	if(semctl(sem_id, order, SETVAL, sem_union) == -1){
		return 0;
	}else{
		return 1;
	}
}

int del_semaphore(){
	int sem_id = semget(sem_key, 3, 0666 | IPC_CREAT);

	union semun sem_union;
	if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1){
		printf("failed to delete semaphore\n");
		return 0;
	}else{
		return 1;
	}
}

int get_semaphore(int order){
	int sem_id = semget(sem_key, 3, 0666 | IPC_CREAT);

	union semun sem_union;
	return semctl(sem_id, order, GETVAL, sem_union);
}

// P操作.
// 尝试获取信号量集中第order个信号量
int semaphore_p(int order){
	int sem_id = semget(sem_key, 3, 0666 | IPC_CREAT);

	struct sembuf sem;
	sem.sem_num = order;
	sem.sem_op = -1;
	sem.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem, 1) == -1){
		printf("semaphore_p failed\n");
		return 0;
	}else{
		return 1;
	}
}

// V操作
// 释放信号量集中第order个信号量
int semaphore_v(int order){
	int sem_id = semget(sem_key, 3, 0666 | IPC_CREAT);

	struct sembuf sem;
	sem.sem_num = order;
	sem.sem_op = 1;
	sem.sem_flg = SEM_UNDO;
	if(semop(sem_id, &sem, 1) == -1){
		printf("semaphore_v failed\n");
		return 0;
	}else{
		return 1;
	}
}