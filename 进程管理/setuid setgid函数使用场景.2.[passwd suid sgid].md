# setuid setgid函数使用场景[suid sgid]

参考文章

1. [Linux中的setuid和setgid](https://blog.csdn.net/whu_zhangmin/article/details/21973201)
    - 以`passwd`命令为例, 解释`suid`的使用场景
    - 设置`suid`的方法
    - 禁用`suid`的方法: `nosuid`
2. [Linux权限管理：setUID、setGID 和 Sticky BIT](https://www.cnblogs.com/qiuyu666/p/11818730.html)
    - `sticky BIT`粘滞位的解释的设置方法
3. [linux c setuid函数解析](https://blog.csdn.net/whatday/article/details/102844879)
    - `setuid()`函数的使用详解
4. [linux setuid函数_setuid函数解析](https://blog.csdn.net/weixin_33744799/article/details/114726191)
    - 参考文章3的转载文章

有了上一篇的基础, 我们再来看一看`setuid()`的作用. 

```c++
#include <sys/types.h>
#include <unistd.h>

int setuid(uid_t uid);
```

APUE(UNIX环境高级编程)对此函数有如下描述:

1. 若进程具有超级用户特权, 则`setuid`函数将实际用户ID, 有效用户ID, 以及保存的设置-用户-ID设置为uid(`setuid()`的参数);
    - 以`root`身份执行, `setuid(uid)`中`uid`可以为任意值, `ruid`, `suid`, `euid`的值都将被修改成`uid`, 很容易理解.
2. 若进程没有超级用户特权, 但是`uid`等于实际用户ID或保存的设置-用户-ID, 则`setuid`只将有效用户ID设置为`uid`, 不改变实际用户ID和保存的设置-用户-ID;
    - 进程以普通用户身份执行, `setuid(uid)`中`uid`可以为成普通用户自身的`uid`, 不过这种其实相当于啥也没干.
    - 进程以普通用户身份执行, 如果该二进制设置了`suid`标记, 且`setuid(uid)`中`uid`等于`suid`标记的用户属主的值, 不过这种其实也相当于啥也没干(因为这种情况下本来`euid`就等于`suid`标记的用户属主).
    - 进程以普通用户身份执行, 如果该二进制设置了`suid`标记, `setuid(uid)`中`uid`可以设置为普通用户自身的`uid`...**这是唯一有意义的角色切换**.
3. 如果上面两个条件都不满足, 则`errno`设置为`EPERM`, 并返回出错;
    - 进程以普通用户身份执行, 但是`setuid(uid)`中`uid`既不等于`suid`标记的属主(如果有的话), 也不等于自身的uid.

> 上面提到的"具有超级用户特权", 即该进程以`root`身份执行, 也即`ruid`为0.

简单概括一下, 以`root`身份运行代码时, `setuid(uid)`中的`uid`可以随意取; 

普通用户执行一个拥有`suid`标记的二进制程序, 最多只能拥有该`suid`属主的权限, 想修改成其他人的uid可不行;
