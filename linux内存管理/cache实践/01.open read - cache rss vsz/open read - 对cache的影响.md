参考文章

1. [Linux C语言中的read write lseek的使用](https://blog.csdn.net/psr1999/article/details/53014092)


使用如下命令创建一个4GB的大文件.

```
dd if=/dev/urandom of=/tmp/ddfile bs=1M count=4096
```


## 实验1

编译并执行`main.1.c`, `open()`打开一个文件, 并不会影响操作系统的cache数据, 只有在`read()`操作时才会.

另外, `read()`虽然会使OS的cache数量, 但这个数量并不会反应在进程本身的内存中, 不管是使用ps还是top, 都没有办法在指标中看到(CODE, DATA, USED 等都没有发生变化). 

所以这部分cache其实是OS自身维护的, 并不存在进程的堆栈中.

------

但是, 在执行`read()`后, OS的cache只增加了1.8G, 和文件本身的大小根本对不上, 这还是个问题...

## 实验2

编译并执行`main.1.c`, 写文件并不会影响cache增加的数量, 仍然是1.8G左右.
