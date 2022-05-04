1. [hoytech/vmtouch](https://github.com/hoytech/vmtouch)
    - `vmtouch`源码
2. [vmtouch - the Virtual Memory Toucher](https://hoytech.com/vmtouch/)
    - 使用示例
3. [cachestat、cachetop、pcstat-linux系统缓存命中率分析工具](https://blog.51cto.com/liuzhengwei521/2360734)
    - 只是命中率统计, 感觉没什么意义
    - `pcstat`和`vmtouch`功能很像, 不知道有没有后者这么强大

这个工具很好使，对查找定位问题很有帮助，特别是对大量使用mmap的系统，比如搜索引擎中的索引。

vmtouch的源文件只有一个c文件，很简单；主要使用了两个系统函数

```c++
// get information on whether pages are in core
int mincore(void *start, size_t length, unsigned char *vec)
```

判断文件数据页是否已经存在于内存中，使用这个函数可以知道一个文件有多少数据已经在page-cache中了。

```c++
// predeclare an access pattern for file data
int posix_fadvise(int fd, off_t offset, off_t len, int advice)
```

声明文件的预访问模式，其中有一个模式是`POSIX_FADV_DONTNEED`，使用这个模式，系统会尽量把该文件的数据从page-cache中踢出去。

## 使用方法

### 查看目标文件在缓存中的大小情况

```console
$ vmtouch ./main
           Files: 1                     ## 文件还是目录?
     Directories: 0
  Resident Pages: 3/3  12K/12K  100%    ## 在page cache中的页数, 大小, 占比(与自身大小相比)
         Elapsed: 0.000103 seconds      ## 
```

执行`echo 3 > /proc/sys/vm/drop_caches`, 可以发现`Resident Pages`被清零.

### `-t`选项, 将目标文件存入page cache

当一个文件并未在page cache中

```console
$ vmtouch ./main
           Files: 1
     Directories: 0
  Resident Pages: 0/3  0/12K  0%
         Elapsed: 0.00048 seconds
```

执行`vmtouch -t`选项

```
$ vmtouch -t ./main
           Files: 1
     Directories: 0
   Touched Pages: 3 (12K)
         Elapsed: 0.00092 seconds
```

再次查看, 发现该文件已经全部存在于page cache中.

```
$ vmtouch ./main
           Files: 1
     Directories: 0
  Resident Pages: 3/3  12K/12K  100%
         Elapsed: 5.2e-05 seconds
```

------

加载一个大文件, 然后用`free`命令看一下

```console
$ echo 3 > /proc/sys/vm/drop_caches
$ free -m
              total        used        free      shared  buff/cache   available
Mem:           3770        1150        2335         100         284        2310
Swap:             0           0           0
$ vmtouch -t ./ddfile
           Files: 1
     Directories: 0
   Touched Pages: 1048576 (4G)
         Elapsed: 18.51 seconds
$ free -m
              total        used        free      shared  buff/cache   available
Mem:           3770        1152         112         100        2505        2292
```

目标文件`ddfile`是我用`dd if=/dev/urandom of=/tmp/ddfile bs=1M count=4096`创建的大文件, 但是全部加载到page cache后, `free`的`buff/cache`列只增加了2G左右...

### `-e`选项, 将目标文件从page cache中移除

类似于`echo 3 > /proc/sys/vm/drop_caches`, 但是这个选项可以从page cache中清除指定的文件, 而不是清空全部缓存.

```console
$ vmtouch -e ./main
           Files: 1
     Directories: 0
   Evicted Pages: 3 (12K)
         Elapsed: 2.3e-05 seconds
$ vmtouch ./main
           Files: 1
     Directories: 0
  Resident Pages: 0/3  0/12K  0%
         Elapsed: 5.3e-05 seconds
```
