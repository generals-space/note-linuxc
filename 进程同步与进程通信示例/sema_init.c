#include "sema_common.h"

void main(){
    int i;
    int sem_id, shm_id;
    void *share_buf = (void *)0;
    share_status *shared_status;
    // 创建的信号量集和共享内存在显式调用del操作删除之前是不会被回收的, 
    // 可以使用ipcs命令查看

    // 信号量和共享内存的操作主键可以认为是sem_id和shm_id, 但是这两个值在函数返回前是无法确定的.
    // 事实上通过同一个sem_key和shm_key也可以得到同一个信号量集或同一块共享内存.
    // 所以我把sem_key和shm_key放到common文件中, 让init, producer, consumer使用同样的key参数.

    // 创建信号量集, 包含3个信号量.
    sem_id = semget(sem_key, 3, 0666 | IPC_CREAT);
    printf("sem id: %d\n", sem_id);

    // 初始化信号量集
    set_semaphore(0,1); // mutext, 控制信号量
    set_semaphore(1,N); // empty
    set_semaphore(2,0); // full

    // 创建共享内存区
    shm_id = shmget(shm_key, sizeof(share_status), 0666 | IPC_CREAT);
    printf("sem id: %d\n", shm_id);

    share_buf = shmat(shm_id, (void *)0, 0);
    printf("attach to %X\n", (int)share_buf);
    shared_status = (share_status *)share_buf;

    // 初始化共享内存区
    shared_status->in = 0;
    shared_status->out = 0;
    for(i = 0; i < N; i ++){
        shared_status->buffer[i] = 0;
    }

    printf("There is %d resource avaliable\n", get_semaphore(2));
}
