
参考文章

1. [自学:pthread_create函数和向线程函数传递参数](https://www.jianshu.com/p/092a073f8984)
    - pthread_create 可用的两种向线程函数传参的方法.
2. [Linux 循环创建多个线程](https://www.cnblogs.com/love-DanDan/p/8724245.html)


参考文章1中所说的传参方法貌似有bug, 用for循环创建线程时会出现.

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define            THREADS          3

void *thread_fn(void *arg)
{
    int order = *(int *)arg;
    // pthread_self() 获取的线程id与 pthread_create()时第一个参数被写入的值相同.
    printf("我是第 %d 个线程，我的线程ID = %lu\n", order, pthread_self());
    return NULL;
}

int main(int argc, char *argv[])
{
    // 定义线程id数组
    pthread_t tid[THREADS]; 

    for(int i = 0; i < THREADS; i ++)
    {
        // 这里写入的 tid[i] 与该线程通过 pthread_self() 获取的线程id相同.
        pthread_create(&tid[i], NULL, thread_fn, &i);
    }
    // 不加 sleep 时, thread_fn 的打印结果可能会有出入, 因为执行太快了,
    // 但仍然是错的.
    sleep(3);

    for(int i = 0; i < THREADS; i ++)
    {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
```

编译方法

```
$ gcc -o thread -lpthread thread.c
```

执行结果

```
$ ./thread
我是第 3 个线程，我的线程ID = 123145468653568
我是第 3 个线程，我的线程ID = 123145469726720
我是第 3 个线程，我的线程ID = 123145469190144
```

上面的传参方法其实最容易理解, 只是会出问题. 

这种情况在很多高级语言中都会出现, 原因就在于变量i的最终赋值与子线程延迟执行.

我在参考文章2中找到了解决方法, 只需要修改`pthread_create`和线程函数中获取参数的部分.

1. main函数中, `pthread_create(&tid[i], NULL, thread_fn, (void *)i);`
2. 线程函数thread_fn中, `int order = (int)arg;`

这种传参与取值的方法看起来有点奇怪, 但却是正确的...

```
$ ./thread
我是第 0 个线程，我的线程ID = 123145316372480
我是第 1 个线程，我的线程ID = 123145316909056
我是第 2 个线程，我的线程ID = 123145317445632
```

不过这只是传递单个参数的情况, 我还没实验过多个参数的传递, 不知道这种情况会不会有什么不同.
