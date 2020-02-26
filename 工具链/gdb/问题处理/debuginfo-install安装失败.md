# debuginfo-install安装失败

参考文章

1. [gdb调试时的问题Missing separate debuginfos, use: debuginfo-install glibc-XXX](https://www.cnblogs.com/wuchanming/p/4492388.html)
2. [centos7 安装 debuginfo](https://www.jianshu.com/p/5b4ef8112b97)

在gdb命令行中, `list`, `breakpoint`执行正常, 但是在使用`run`命令准备开始执行时出现错误.

```console
(gdb) b 405
Breakpoint 1 at 0x4013c5: file rbtree.c, line 405.
(gdb) r
Starting program: /home/project/playground/rbtree/./rbtree

Breakpoint 1, main () at rbtree.c:405
405	        T = rbt_insert(T, list[i]);
Missing separate debuginfos, use: debuginfo-install glibc-2.17-292.el7.x86_64
```

尝试执行`debuginfo-install glibc`完成安装, 失败.

```console
$ debuginfo-install glibc
已加载插件：fastestmirror, langpacks
enabling epel-debuginfo
enabling docker-ce-stable-debuginfo
Loading mirror speeds from cached hostfile
 * base: mirrors.aliyun.com
 * epel: mirrors.aliyun.com
 * epel-debuginfo: mirrors.aliyun.com
 * extras: mirrors.aliyun.com
 * updates: mirrors.aliyun.com
Could not find debuginfo for main pkg: glibc-2.17-292.el7.x86_64
Could not find debuginfo pkg for dependency package nss-softokn-freebl-3.44.0-5.el7.x86_64
Could not find debuginfo for main pkg: glibc-2.17-292.el7.i686
Could not find debuginfo pkg for dependency package nss-softokn-freebl-3.44.0-5.el7.i686
No debuginfo packages available to install
```

查了查, 发现需要在`debuginfo.repo`源文件中把`enable`字段修改为1.

我使用的是阿里云的镜像, 但并没有`debuginfo`工具可用的源(`epel.repo`中有一个`epel-debuginfo`, 但是开启了没用).

按照参考文章2中所说, 手动创建`debuginfo.repo`, 内容如下

```
#Debug Info
[debug]
name=CentOS-$releasever - DebugInfo
baseurl=http://debuginfo.centos.org/$releasever/$basearch/
gpgcheck=0
enabled=1
protect=1
priority=1
```

再次执行`debuginfo-install glibc`, 成功. gdb命令行中的`run`也成功了, 无需退出.

```console
(gdb) r
Starting program: /home/project/playground/rbtree/rbtree

Breakpoint 1, main () at rbtree.c:405
405	        T = rbt_insert(T, list[i]);
Missing separate debuginfos, use: debuginfo-install glibc-2.17-292.el7.x86_64
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n)
```

