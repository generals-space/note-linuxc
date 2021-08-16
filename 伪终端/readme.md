参考文章

1. [ptsname() -- 获得从伪终端名(slave pseudo-terminal)](https://blog.csdn.net/heron804/article/details/8144103)
    - 提供了2种打开pts终端的方式: 
        1. open(ptmx) -> ptsname(ptmx)
        2. opentty()
2. [Linux 的伪终端的基本原理 及其在远程登录（SSH，telnet等）中的应用](https://www.cnblogs.com/zzdyyy/p/7538077.html)
3. [linux下伪终端机制详解](https://wenku.baidu.com/view/53d0daf8aef8941ea76e05d2.html)
    - `grantpt()`, `unlockpt()`, `ptsname()`都是标准库提供的函数.

```
// 如果不加 _XOPEN_SOURCE , 貌似对 ptsname(), unlockpt() 函数的代码提示会不正常.
#define _XOPEN_SOURCE
```
