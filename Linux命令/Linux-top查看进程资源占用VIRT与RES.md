# Linux-top查看进程资源占用VIRT与RES

<!tags!>: <!虚拟内存!> <!物理内存!> <!共享内存!>

参考文章

1. [linux top命令看到的实存(RES)与虚存(VIRT)分析](https://www.cnblogs.com/xudong-bupt/p/8643094.html)
    - 解释了top命令中`VIRT(虚拟内存)`, `RES(常驻内存)`与`SHR(共享内存)`的含义
    - 配有2个c语言示例, 展示了申请内存, 与写入数据后`VIRT`和`RES`的值的变化. 示例简单清晰, 值得一看.
2. [linux top命令VIRT,RES,SHR,DATA的含义](https://javawind.net/p131)
    - 介绍了top命令中`VIRT`, `RES`与`SHR`和`DATA`字段的含义.
    - 介绍了top命令所有可用字段的含义, 相同于top命令的高级使用手册
3. [Linux交换空间（swap space）](https://segmentfault.com/a/1190000008125116)
    - 关于swap的原理和作用讲述得比较清晰易懂
    - 参考文章1,2都有提到swap out操作, 可以阅读本文理解其过程.
4. [DevOps Top和strace 查看某些或者某个进程](https://www.dazhuanlan.com/lock327/topics/1265904)
    - `top -p pid1,pid2`可以查看指定进程

按照参考文章2所说, 可使用`-f`选项额外显示`Swap`, `Code`, `Data`, `Used`列, 与本文的目的大致相关. 

- `VIRT`: `virtual memory usage` 虚拟内存, 等同于`ps aux`结果中的`VSZ`列. 
    - 虚拟内存会映射到swap空间(想想win下虚拟内存的设置, linux也是一样的道理)
    - VIRT=SWAP+RES
    - `SWAP`是虚拟内存被换出的大小, 但是虚拟内存可以很大, 远远超过swap的大小. 
    - 所以被swap out的空间应该不会是malloc向系统申请的空间, 而是当前进程不常用其他数据和库.
- `RES`: `resident memory usage` 常驻内存, 等同于`ps aux`结果中的`RSS`列. 
    - 由于被称为常驻内存, 所以其与swap是不相交的, 但ta们的内容并非是不相容的. 
    - ~~RES=CODE+DATA~~ 按照下面的示例, 此公式并不成立
- `SHR`: `shared memory` 共享内存.
- `SWAP`: 进程使用的虚拟内存中, 被换出的大小.
- `CODE`: 可执行代码占用的物理内存大小
- `DATA`: 可执行代码以外的部分(数据段+栈)占用的物理内存大小. 
- `USED`: RES+SWAP.

有点乱, 这里保留疑问, 日后再来研究吧<???>

参考文章1给出的第一个示例使用了C++的语法: `new`和`delete`, 这里我改成纯C语言的实现.

`main.c`

```c++
// 这里引用 log.c 代码, 拥有日志分级打印的功能.
int main()
{
    int test = 0;
    // 分配512M(堆内存), 未使用
    char* p;
    log_info("before malloc");
    scanf("%d", &test); //等待输入

    p = malloc(1024 * 1024 * 512);
    log_info("after malloc, try to use 10M");
    scanf("%d", &test); //等待输入

    // 使用10M
    memset(p, 0, 1024 * 1024 * 10);

    log_info("try to use another 50M");
    scanf("%d", &test); //等待输入

    // 使用50M
    memset(p, 0, 1024 * 1024 * 50);

    log_info("try to use all");
    scanf("%d", &test); //等待输入

    memset(p, 0, 1024 * 1024 * 512);

    log_info("complete, exit");
    scanf("%d", &test); //等待输入

    return 0;
}
```

编译并执行

```
gcc -g -o main main.c
./main
```

使用`top/ps`命令查看.

在一切操作之前

```
   PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND SWAP   CODE    DATA   USED
 52804 root      20   0    4352    524    432 S   0.0  0.0   0:00.00 main       0      4     320    524
[root@k8s-master-01 ~]# ps -o pid,%cpu,%mem,rss,vsz,drs,trs,size,start_time,command -p 52804
   PID %CPU %MEM   RSS    VSZ   DRS  TRS  SIZE START COMMAND
 52804  0.0  0.0   524   4352  4348    3   320 21:36 ./main
```

在堆上分配512M空间后

```
   PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND SWAP   CODE    DATA   USED
 52804 root      20   0  528644    524    432 S   0.0  0.0   0:00.00 main       0      4  524612    524
[root@k8s-master-01 ~]# ps -o pid,%cpu,%mem,rss,vsz,drs,trs,size,start_time,command -p 52804
   PID %CPU %MEM   RSS    VSZ   DRS  TRS  SIZE START COMMAND
 52804  0.0  0.0   524 528644 528640   3 524612 21:36 ./main
```

可以看到, `VIRT/VSZ`直接增加了512M, 但是`RES/RSS`一点没变, 因为`malloc`划分的内存在没有实际写入内存的时候都是虚的, 根本没有分配实际的物理内存.

另外`DATA`字段也增加了相应的大小.

使用10M后

```
   PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND SWAP   CODE    DATA   USED
 52804 root      20   0  528644  11548    516 S   0.0  0.3   0:00.00 main       0      4  524612  11548
[root@k8s-master-01 ~]# ps -o pid,%cpu,%mem,rss,vsz,drs,trs,size,start_time,command -p 52804
   PID %CPU %MEM   RSS    VSZ   DRS  TRS  SIZE START COMMAND
 52804  0.0  0.2 11548 528644 528640   3 524612 21:36 ./main
```

这次`VIRT/VSZ`没变, `RES/RSS`增加了10M.

使用50M后

```
   PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND SWAP   CODE    DATA   USED
 52804 root      20   0  528644  52512    516 S   0.0  1.4   0:00.02 main       0      4  524612  52512
[root@k8s-master-01 ~]# ps -o pid,%cpu,%mem,rss,vsz,drs,trs,size,start_time,command -p 52804
   PID %CPU %MEM   RSS    VSZ   DRS  TRS  SIZE START COMMAND
 52804  0.0  1.3 52512 528644 528640   3 524612 21:36 ./main
```

注意, 本次`RES/RSS`相比上面增加了10M, 与最开始相比则为50M.

## 2. 在栈上分配

参考文章1中的第二个示例在栈上申请了20M空间, 但是在CentOS7服务器版上, `ulimit -s`所规定的栈空间为8M, 所以在编译完成然后执行时出现了`Segmentation fault (core dumped)`.

我尝试过为`p`数组分配8M内存, 然后...果然失败了. 因为栈空间不只包含这个数组, 还有其他代码, 所以我试着分配了7M内存, 可以了...

为了数据展示更友好, 这里为`p`数组分配5M内存.

```c++
#include <stdio.h>
#include <string.h>

int main()
{
    int test = 0;
    // 20M栈, 未使用
    // char p[1024 * 1024 * 20];
    char p[1024 * 1024 * 5];
    scanf("%d", &test);    //等待输入

    // 使用10M
    // memset(p, 0, 1024 * 1024 * 10);
    memset(p, 0, 1024 * 1024 * 3);
    scanf("%d", &test);    //等待输入
    return 0;
}
```

编译并执行

```
gcc -g -o stack stack.c
./stack
```

在栈上分配5M空间后

```
  PID USER        VIRT    RES    SHR %CPU %MEM COMMAND SWAP   CODE    DATA   USED
12855 root        9216    348    272  0.0  0.0 stack      0      4    5188    348
```

使用3M后

```
  PID USER        VIRT    RES    SHR %CPU %MEM COMMAND SWAP   CODE    DATA   USED
12855 root        9216   3512    384  0.0  0.3 stack      0      4    5188   3512
```
