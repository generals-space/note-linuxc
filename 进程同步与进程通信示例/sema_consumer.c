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
        shared_status->out ++;

        printf("Process %d get a resource and the sum is%d\n", getpid(), get_semaphore(2) - 1);
        printf("the buffer pool condition:");
        for(i = 0; i < N; i ++){
            printf("%d",shared_status->buffer[i]);
        }
        printf("\n");

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