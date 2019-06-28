#include "sema_common.h"

void main(){
    int i;
    int shm_id;
    void *share_buf = (void *)0;
    share_status *shared_status;

    shm_id = shmget(shm_key, sizeof(share_status), 0666 | IPC_CREAT);
    share_buf = shmat(shm_id, (void *)0, 0);
    printf("attach to %X\n", (int)share_buf);
    shared_status = (share_status *)share_buf;

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

        shared_status->in = shared_status->in % N;
        shared_status->buffer[shared_status->in] = 1;
        shared_status->in ++;

        printf("Process %d put a resource and the sum is %d\n ", getpid(), get_semaphore(2) + 1);
        printf("the buffer pool condition:");
        for(i = 0; i < N; i ++){
            printf("%d",shared_status->buffer[i]);
        }
        printf("\n");

        sleep(rand() % 3);

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
}