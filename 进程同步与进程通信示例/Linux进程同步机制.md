# Linux进程同步机制与通信机制

参考文章

1. [进程间通信之-信号量semaphore--linux内核剖析（十）](https://kernel.blog.csdn.net/article/details/50994533)

以linux下的gcc为编译器, 调用linux提供的多种系统调用, 实现进程管理相关功能, 包括: 

1. 进程创建及撤销. 实现多个进程的创建、执行及撤销. 
2. 进程同步. 利用linux的信号量集机制, 实现生产者—消费者问题. 
3. 进程通信. 创建两个以上进程, 实现多个进程之间的管道通信（匿名管道及命名管道）、消息队列通信、共享存储器通信三种通信机制. 

## 1. 进程创建及撤销

```c
pid_t fork(void);
```

进程创建函数, 与之有相同作用的还有vfork();

`pid_t` 是一个宏定义, 其实质是int 被定义在`#include<sys/types.h>`中.

返回值:  若成功调用一次则返回两个值, 子进程返回0, 父进程返回子进程ID；否则, 出错返回-1

## 2. 进程同步

关键函数:

```c
int semget(key_t key, int nsems, int semflg);
```

创建信号量集, 返回其相应标识.

semflg可用值为`IPC_CREAT`和`IPC_EXCL`, 一般是单独使用`IPC_CREAT`或是`IPC_CREAT|IPC_EXCL`. 前者当信号量不存在时创建一个新的信号量, 已存在时也不会出错. 后者可以自动创建新的信号量, 但是如果目标信号量已存在, 则会失败, 返回-1.

```c
int semop（int semid, struct sembuf *sops, size_t nsops）;
```

对信号量集进行操作, 可实现pv操作.

```c
int semctl(int semid, int semnum, int cmd, ...);
```

对信号量集进行控制操作, 可实现删除信号量集的功能.

cmd: 操作类型, 可选值有`GETVAL`, `SETVAL`, `IPC_RMID`.

## 3. 进程通信

### 3.1 共享内存机制

```c
int shmget(key_t key, size_t size, int shmflg)
```

得到一个共享内存标识符或创建一个共享内存对象并返回共享内存标识符

```c
void *shmat(int shmid, const void *shmaddr, int shmflg)
```

连接共享内存标识符为`shmid`的共享内存, 连接成功后把共享内存区对象映射到调用进程的地址空间, 随后可像本地空间一样访问

```c
int shmdt(const void *shmaddr)
```

与shmat函数相反, 是用来断开与共享内存附加点的地址, 禁止本进程访问此片共享内存

```c
int shmctl(int shmid, int cmd, struct shmid_ds *buf)
```

完成对共享内存的控制.

cmd参数的可用值.

- IPC_STAT: 得到共享内存的状态, 把共享内存的shmid_ds结构复制到buf中
- IPC_SET: 改变共享内存的状态, 把buf所指的shmid_ds结构中的uid、gid、mode复制到共享内存的shmid_ds结构内
- IPC_RMID: 删除这片共享内存

### 3.2 消息队列机制

```c
int msgget(key_t key, int msgflg);
```

msgget()函数的第一个参数是消息队列对象的关键字(key), 函数将它与已有的消息队列对象的关键字进行比较来判断消息队列对象是否已经创建. 而函数进行的具体操作是由第二个参数, msgflg 控制的. 它可以取下面的几个值: 

- IPC_CREAT: 如果消息队列对象不存在, 则创建之, 否则则进行打开操作;
- IPC_EXCL: 和IPC_CREAT 一起使用（用"|"连接）, 如果消息对象不存在则创建之, 否则产生一个错误并返回. 

```c
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
```

msgrcv()可以从消息队列中读取消息, msgsnd()将一个新的消息写入队列. 

为了发送消息, 调用进程对消息队列进行写入时必须有写权能. 接收消息时必须有读权能. 

### 3.3 命名管道:

```c
int mkfifo(const char * pathname,mode_t mode);
```

`mkfifo()`会依参数pathname建立特殊的FIFO文件, 该文件必须不存在, 而参数mode为该文件的权限（mode%~umask）, 因此 umask值也会影响到FIFO文件的权限. `mkfifo()`建立的FIFO文件其他进程都可以用读写一般文件的方式存取. 当使用open()来打开 FIFO文件时, `O_NONBLOCK`标记会有影响

1. 当使用`O_NONBLOCK`标记时, 打开FIFO文件来读取的操作会立刻返回, 但是若还没有其他进程打开FIFO文件来读取, 则写入的操作会返回ENXIO 错误代码. 
2. 没有使用`O_NONBLOCK`标记时, 打开FIFO来读取的操作会等到其他进程打开FIFO文件来写入才正常返回. 同样地, 打开FIFO文件来写入的操作会等到其他进程打开FIFO文件来读取后才正常返回. 

若成功则返回0, 否则返回-1, 错误原因存于errno中. 
