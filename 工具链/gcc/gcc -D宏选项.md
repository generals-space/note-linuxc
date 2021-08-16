# gcc -D宏选项

参考文章

1. [O_CLOEXEC模式和FD_CLOEXEC选项](https://blog.csdn.net/ubuntu_hao/article/details/51393632)

宏选项感觉很像环境变量, 不过宏选项是在gcc编译时就确定下来的, 无法更改. 一般是按照编译平台的不同, 编译时的代码块也会不同.

```c++
#ifdef _O_CLOEXEC
    if ((fd = open("my.txt", O_RDWR | O_CREAT | O_CLOEXEC, 0600)) < 0) 
#else
    if ((fd = open("my.txt", O_RDWR | O_CREAT, 0600)) < 0) 
#endif
```

上述代码中, `_O_CLOEXEC`就是一个宏选项, 编译时可以使用`gcc -D_O_CLOEXEC -o xxx xxx.c`通过是否添加`_O_CLOEXEC`宏选项, 来决定使用哪个if语句.
