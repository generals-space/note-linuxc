# fork vfork clone的联系与区别

参考文章

1. [fork()、vfork()、clone()的区别](https://blog.csdn.net/gogokongyin/article/details/51178257)
    - 值得收藏
    - （Copy-On-Write）写时复制: 在`fork`之后`exec`之前, 两个进程用的是相同的物理空间（内存区），先把页表映射关系建立起来，并不真正将内存拷贝.
    - `vfork`创建的子进程要先于父进程执行，子进程执行时，父进程处于挂起状态，子进程执行完，唤醒父进程。除非子进程`exit`或者`execve`才会唤起父进程
    - `clone`是`fork`的升级版本，不仅可以创建进程或者线程，还可以指定创建新的命名空间（namespace）、有选择的继承父进程的内存、甚至可以将创建出来的进程变成父进程的兄弟进程等等。
2. [Linux中fork，vfork和clone详解（区别与联系）](https://blog.csdn.net/gatieme/article/details/51417488)
3. [linux下的进程（process）相关API函数(创建（fork vfork clone）、回收（wait\waitpid）、退出（exit\_exit）)及软件开发相关面试问题](https://blog.csdn.net/jmw1407/article/details/106961781)
    - `clone`的返回值是子进程的pid，出错的话返回－1，设置errno，并不建立子进程
