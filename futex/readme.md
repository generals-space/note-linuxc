# futex

参考文章

1. [Futex设计与实现](https://www.jianshu.com/p/d17a6152740c)
2. [undefined reference 'shm_open', already add -lrt flag here](https://stackoverflow.com/questions/9923495/undefined-reference-shm-open-already-add-lrt-flag-here)
    - gcc编译选项需要添加`-lrt`
3. [Futex system call](https://www.linuxquestions.org/questions/linux-software-2/futex-system-call-805006/)
4. [ Linux Programmer's Manual - FUTEX(2)](http://man7.org/linux/man-pages/man2/futex.2.html)

futex的wait操作可以在指定地址上休眠, 且原子地检测该地址指向的值是否发生变动, 如果变动则返回. 也可以设置超时时间返回; wakeup操作可以唤醒在指定地址上休眠的进程, 可以指定可唤醒的进程的数量.

本示例由主进程创建共享内存, 然后fork出一个子进程, 父进程调用`futex`一直休眠等待. 直到子进程向共享内存中写入数据完成后, 调用futex将父进程唤醒. 父进程苏醒后, 打印共享内存中的内容. 流程结束.

听起来像是golang的Condition或是python的中Event一样, 实现了线程间的简单通信...

## 部署及实践

```
docker run -it --name futex -v .:/app generals/centos7 /bin/bash
```

编译

```
gcc -g -std=c99 -lrt -o main main.c
```

- `-std=c99`: for语句中初始化计数变量需要c99标准支持
- `-lrt`: 编译`shm_open`函数必需(man手册中有提到), 如果不加, 可能出现`undefined reference to 'shm_open'`的编译错误.
