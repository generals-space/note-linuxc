# GDB调试多线程程序

参考文章

1. [docker下gdb调试断点不停](https://blog.csdn.net/so_dota_so/article/details/77509530)
2. [gdb调试多进程与多线程](https://blog.csdn.net/snow_5288/article/details/72982594)
    - `set follow-fork-mode child`+`set detach-on-fork off`, ...不太行的样子

在gdb中设置`set follow-fork-mode child`使gdb既能调试父进程, 又能调试子进程.

```
(gdb) set follow-fork-mode child
(gdb) r
Starting program: /root/hello
...
```
