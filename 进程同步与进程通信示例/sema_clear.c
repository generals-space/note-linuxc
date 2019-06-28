#include "sema_common.h"

void main(){
    int shm_id;
    void *share_buf = (void *)0;
    share_status *shared_status;
    int shm_result;
    shm_id = shmget(shm_key, sizeof(share_status), 0666 | IPC_CREAT);
    share_buf = shmat(shm_id, (void *)0, 0);
    printf("attach to %X\n", (int)share_buf);
    shared_status = (share_status *)share_buf;

    del_semaphore();
    // 与shmat相反, 断开共享内存连接
	shmdt(shared_status);
    // shmctl的IPC_RMID才是真正将这一共享内存清除的方法.
    shm_result = shmctl(shm_id, IPC_RMID, NULL);
}
