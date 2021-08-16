参考文章

1. [Linux系统编程之错误处理：perror,strerror和errno](http://www.linuxidc.com/Linux/2013-07/87238.htm)

注意: strerror() 需要同时引入下面两个库, 否则printf()会出现段错误.

```c++
        ns_fd = open(ns_path, O_RDONLY);
        if(ns_fd < 0){
            printf("failed to open ns file %s: %s\n", ns_path, strerror(errno));
            exit(1);
        }
```
