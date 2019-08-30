/* 
 * This sample show how to use futex betwen two process, and use system v
 * shared memory to store data 
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <linux/futex.h>
#include <fcntl.h>
#include <errno.h>

#if __GLIBC_PREREQ(2, 3)    
#if defined FUTEX_WAIT || defined FUTEX_WAKE 
#include <linux/futex.h>
#else
#define FUTEX_WAIT      0
#define FUTEX_WAKE      1
#endif

#ifndef __NR_futex
#define __NR_futex     202
#endif
#endif

struct namelist 
{
    int  id; 
    char name[20];
};
typedef struct namelist namelist;

int main(void)
{
    int fd, pid, status;
    int *ptr;
    struct stat stat;

    int size = 100;
    // 创建共享内存对象
    // 虽然shmfile像是一个文件路径, 但并不是在文件系统中创建的.
    char shmfile[] = "/tmp";
    int shm_flag = O_RDWR | O_CREAT;
    mode_t shm_mode = S_IRUSR | S_IWUSR;

    pid = fork();
    if (pid > 0) { 
        // 父进程

        // 父子进程各自使用shm_open()打开/创建共享内存对象, 
        // 由于shnfile值相同, 所以打开的一定是同一个.
        fd = shm_open(shmfile, shm_flag, shm_mode);
        if (fd < 0)
        {
            printf("shm_open failed, errormsg=%s errno=%d", strerror(errno), errno);
            return 0;
        }
        // ftruncate 将参数fd指定的文件大小改为参数size指定的大小
        // 一般用在文件初始化或者重新为文件分配空间时.
        ftruncate(fd, size);
        ptr = (int *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);

        printf("parent start waiting\n");
        syscall(__NR_futex, ptr, FUTEX_WAIT, *(int *)ptr, NULL);
        printf("parent end waiting\n");

        namelist tmp;

        int total = *ptr;
        printf("\nThere is %d item in the shm\n", total);

        ptr++;
        namelist *cur = (namelist *)ptr;
        // for循环中进行i变量的初始化从C99才开始支持, 编译时需要加上`-std=c99`选项.
        for (int i = 0; i< total; i++) {
            tmp = *cur;
            printf("%d: %s\n", tmp.id, tmp.name);
            cur++;
        }

        printf("\n");
        waitpid(pid, &status, 0);
    } else { 
        // 子进程
        sleep(5);
        printf("Child %d: start\n", getpid());

        fd = shm_open(shmfile, shm_flag, shm_mode);
        fstat(fd, &stat);

        ptr = (int *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
        namelist tmp;

        // 在共享内存区起始部分存入一个整数, 之后存入3个namelist实例对象.
        *ptr = 3;
        // 注意这里赋值了一个新变量, 未改变ptr指向的地址, 因为之后的futex wakeup需要这个ptr.
        namelist *cur = (namelist *)(ptr+1);
        // 看了下面语句中对共享内存的写入操作, 难道mmap返回的是fd文件指针映射?
        tmp.id = 1;
        strcpy(tmp.name, "Nellson");
        *cur = tmp;
        cur ++;
        tmp.id = 2;
        strcpy(tmp.name, "Daisy");
        *cur = tmp;
        cur ++;
        tmp.id = 3;
        strcpy(tmp.name, "Robbie");
        *cur = tmp;

        printf("wake up parent\n");
        // futex的wake up操作, 第3个参数表示可以唤醒的进程数量
        // 如下为1, 那么这条语句可以唤醒1个在ptr地址休眠的进程.
        syscall(__NR_futex, ptr, FUTEX_WAKE, 1, NULL);
        exit(0);
    }

    printf("Parent %d get child status:%d\n", getpid(), status);
    return 0;
}
