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

关于`suid`, `sgid`特殊权限这里就不再叙述了, 我们主要探究一下在 linux c 中, `setuid()`, `setgid()`的使用场景和使用方法.

内核为每个进程维护的三个UID值: 

- ruid(real uid): 实际用户ID
- suid(saved set-user-ID): 保存的设置用户ID
- euid(effective uid): 有效用户ID

以如下代码为例, 直接打印出这3个UID值.

```c++
// setuid_test.c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc,char *argv[])
{
    uid_t ruid, suid, euid;
    getresuid(&ruid,&suid, &euid);
    printf("real: %d\tset-user-id: %d\teffective: %d\n", ruid, suid, euid);
    return 0;
}
```

##

在没有设置该程序的`suid`标记的场景下, 分别以`root`与`general`用户执行.

```console
[root@k8s-master-01]# cat /etc/passwd | grep root
root:x:0:0:root:/root:/bin/bash
[root@k8s-master-01]# cat /etc/passwd | grep general
general:x:1000:1000::/home/general:/bin/bash
```

以`root`身份执行

```console
[root@k8s-master-01]# ./setuid_test
real: 0	set-user-id: 0	effective: 0
```

以`general`普通用户身份执行

```console
[root@k8s-master-01]# su -l general
上一次登录：二 8月 11 17:49:49 CST 2020从 172.16.91.1pts/3 上
su: 警告：无法更改到 /home/general 目录: 没有那个文件或目录
-bash-4.2$ cd /tmp/
-bash-4.2$ ls
setuid_test  setuid_test.c
-bash-4.2$ ./setuid_test
real: 1000	set-user-id: 1000	effective: 1000
```

##

然后回到`root`用户, 为其设置`suid`标记

```
[root@k8s-master-01]# ll
总用量 32
drwxr-xr-x   3 root root    79 8月  29 18:24 .
drwxrwxrwt. 18 root root 81920 8月  29 21:13 ..
-rwxr-xr-x   1 root root  9808 8月  29 18:24 setuid_test
-rw-r--r--   1 root root   266 8月  29 18:23 setuid_test.c
[root@k8s-master-01]# chmod u+s ./setuid_test
[root@k8s-master-01]# ll
总用量 32
drwxr-xr-x   3 root root    79 8月  29 18:24 .
drwxrwxrwt. 18 root root 81920 8月  29 21:13 ..
-rwsr-xr-x   1 root root  9808 8月  29 18:24 setuid_test
-rw-r--r--   1 root root   266 8月  29 18:23 setuid_test.c
```

此时以`root`用户执行此程序没什么变化, 这里不再给出示例, 而以`general`身份执行的代码, 则发生了改变.

```
[root@k8s-master-01 ~]# su -l general
上一次登录：日 8月 29 18:27:19 CST 2021pts/6 上
su: 警告：无法更改到 /home/general 目录: 没有那个文件或目录
-bash-4.2$ cd /tmp/
-bash-4.2$ ls
setuid_test  setuid_test.c
-bash-4.2$ ./setuid_test
real: 1000	set-user-id: 0	effective: 0
```

## 总结

此时回头看上面的3个UID值, 可以有如下认识.

- ruid(real uid): 执行当前进程的用户uid(谁执行了这个进程, 这个uid就是谁).
- suid(saved set-user-ID): 如该程序的二进制文件拥有`suid`标记, 那么此值就是其用户属主的uid; 如果没有, 那就和`ruid`一样.
- euid(effective uid): 最终生效的uid, 一般来说, 应该与`suid`保持一致.
