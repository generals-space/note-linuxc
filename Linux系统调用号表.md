# Linux系统调用号表

参考文章

1. [linux系统调用号查询](https://blog.csdn.net/u012763794/article/details/78777938)
2. [Futex system call](https://www.linuxquestions.org/questions/linux-software-2/futex-system-call-805006/)
3. [ Linux Programmer's Manual - FUTEX(2)](http://man7.org/linux/man-pages/man2/futex.2.html)

系统调用是内核提供的功能函数, 一般可以通过函数名直接调用, 也可以通过`syscall`, 将系统调用函数与数字对应起来调用.

系统调用的映射列表如下.

centos

- 32位: `/usr/include/asm/unistd_32.h`
- 64位: `/usr/include/asm/unistd_64.h`

以64位系统为例

```h
#ifndef _ASM_X86_UNISTD_64_H
#define _ASM_X86_UNISTD_64_H 1

#define __NR_read 0
#define __NR_write 1
#define __NR_open 2
#define __NR_close 3
#define __NR_stat 4
#define __NR_fstat 5
#define __NR_lstat 6
#define __NR_poll 7
#define __NR_lseek 8
#define __NR_mmap 9
```

> ...但是系统调用号表又不能看到各函数的原型, 不知道参数列表怎么调用?

------

好吧我错了...

我曾见过有一段示例代码中使用`syscall`发起系统调用的语句.

```c++
#ifndef __NR_futex
#define __NR_futex     202
#endif
#define FUTEX_WAIT      0
#define FUTEX_WAKE      1

syscall(__NR_futex , ptr, FUTEX_WAIT, *(int *)ptr, NULL );
```

然后尝试直接用系统调用函数替代ta.

```c++
futex(ptr, FUTEX_WAIT, *(int *)ptr, NULL);
```

但是在编译时出现`undefined reference to 'futex'`的报错.

查阅参考文章2, 解决方法就是用`syscall`代替直接`futex`...

好吧, 后来我又找到了参考文章3, 这个的man手册新了很多, 在文末的示例代码中, 直接写了一个`futex`的包装函数, 代为执行`syscall`...

```c++
static int
futex(int *uaddr, int futex_op, int val,
        const struct timespec *timeout, int *uaddr2, int val3)
{
    return syscall(SYS_futex, uaddr, futex_op, val,
                    timeout, uaddr, val3);
}
```

我放弃了.
