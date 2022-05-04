参考文章

1. [合理利用和优化pagecache](http://blog.chinaunix.net/uid-20662820-id-3480240.html)
    - linux对文件的访问除了显示使用`O_DIRECT`标志，所有的访问都会经过pagecache
    - 每个文件在内存中最多有一个pagecache镜像，也就是说**对同一个文件，所有的进程共享pagecache**
    - cache
        - /proc/sys/vm/drop_caches
        - ~~/proc/sys/vm/pagecache_limit_mb~~ centos7(内核 3.10.0)没有此配置
        - `/proc/sys/vm/dirty_ratio`和`pagecache_limit_mb`应该不是一回事, 前者只是指定了写回的阈值, 后者应该是可以限制缓存区的大小的(可惜不能用).
    - `vmtouch`工具提供了查看指定文件的pagecache情况，以及强制对文件进行pagecache和清除pagecache
        - 主要使用了两个系统调用`mincore`和`posix_fadvise`
2. [Page Cache的落地问题](https://cloud.tencent.com/developer/article/1143727)
    - `close()`关闭文件描述符并不会触发"刷盘"
    - 触发刷新脏页的条件：
        1. 调用`fsync`等函数
        2. 脏页太多(`dirty_background_ratio`与`dirty_ratio`)
        3. 脏页太久(`dirty_expire_centisecs`)
    - 对`/proc/sys/vm/`目录下的`dirty_XXX`相关配置进行了解释
3. [pagecache的测试和理解](https://blog.csdn.net/Ives_WangShen/article/details/109114128)
    - pagecache本身是用来优化IO性能的，但是异常断电时会丢数据
4. [linux pagecache限制与查看](https://www.cnblogs.com/10087622blog/p/8191169.html)
    - 提到了多个限制page cache的方法, 很硬核
    - 提到了`dirty_background_ratio`与`dirty_ratio`先后被触发的场景

