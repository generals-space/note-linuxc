# GDB调试多线程程序

参考文章

1. [docker下gdb调试断点不停](https://blog.csdn.net/so_dota_so/article/details/77509530)

在gdb中设置`set follow-fork-mode child`使gdb既能调试父进程, 又能调试子进程.

```
(gdb) set follow-fork-mode child
(gdb) r
Starting program: /root/hello
...
```
