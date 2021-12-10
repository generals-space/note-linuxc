参考文章

1. [[译] linux内存管理之RSS和VSZ的区别](https://www.pengrl.com/p/21292/)
    - RSS: 400K + 1000K + 100K = 1500K
    - VSZ: 500K + 2500K + 200K = 3200K
    - 二进制文件, 进程堆/栈, 以及动态链接库等, 都可以被swap(即不会全在物理内存中)
    - OS中各进程可能引用同样的共享库, 这样各进程的RSS都会将该共享库计入, 所以如果将OS上所有进程的RSS值加起来会大于物理内存
    - 一个进程中的多个线程共享同样的地址空间, 所以一个进程中的多个线程的RSS，VSZ，PSS是完全相同的.
2. [What is RSS and VSZ in Linux memory management](https://stackoverflow.com/questions/7880784/what-is-rss-and-vsz-in-linux-memory-management/21049737#21049737)
    - 参考文章1的原文
3. [A way to determine a process's "real" memory usage, i.e. private dirty RSS?](https://stackoverflow.com/questions/118307/a-way-to-determine-a-processs-real-memory-usage-i-e-private-dirty-rss)
    - 太多资源指标无法反映一个进程的自身占用的资源, 只有`/proc/$pid/smaps`中`Private_Dirty`可以(maybe).
    - 同一个可执行文件的多个进程实例时会共享程序代码
    - 不同进程有可能会引用同样的共享库
    - 不同进程间有可能占用同一块共享内存
    - 虚拟内存机制使VM大小中包含了很多没有被使用的空间
    - 当一个进程被swap out时, ta的rss会变成0
