# set为目标程序设置运行参数args及环境变量environment[print struct]

参考文章

1. [gdb调试——set](https://blog.csdn.net/huqinwei987/article/details/50706743)
2. [gdb设置命令参数和环境变量](https://blog.csdn.net/weixin_43085694/article/details/105331388)

## 场景描述

以前在调试时, 目标程序都不需要参数, 直接`gcc xxx`就可以了.

最近在调试一下第3方的工具(runc), 需要执行ta的一些子命令, 于是执行`gcc runc run container`, 结果调试进去后, 最终给我打印了`help`信息.

```
(gdb) n
NAME:
   runc - Open Container Initiative runtime
...
USAGE:
   runc [global options] command [command options] [arguments...]

VERSION:
   1.0.0-rc8+dev
commit: 6ef6398347c14911251b5b9b8566f592c651f52b-dirty
spec: 1.0.1-dev

COMMANDS:
     checkpoint  checkpoint a running container
     create      create a container
     delete      delete any resources held by the container often used with detached container
...
GLOBAL OPTIONS:
   --debug             enable debug output for logging
   --log value         set the log file path where internal debug information is written
...
[Thread 0x7ffff7feb740 (LWP 101059) exited]
...
[Inferior 1 (process 101059) exited normally]
```

而且调试的时候发现传入的`Args`变量长度只有1, 内容只有`runc`本身的路径.

```
Breakpoint 1, main.main () at /home/runc/main.go:145
145		if err := app.Run(os.Args); err != nil {
Missing separate debuginfos, use: debuginfo-install glibc-2.17-317.el7.x86_64 libseccomp-2.3.1-3.el7.x86_64
(gdb) s
github.com/urfave/cli.(*App).Run (a=0xc00008d1e0, arguments=..., err=...) at /usr/local/gopath/pkg/mod/github.com/urfave/cli@v0.0.0-20170811014203-cfb38830724c/app.go:175
175	func (a *App) Run(arguments []string) (err error) {
(gdb) n
176		a.Setup()
(gdb) p arguments[0]
Attempt to take address of value not located in memory.
(gdb) p arguments
$2 = {array = 0xc0000121d0, len = 1, cap = 1}
(gdb) p *arguments.array
$4 = 0x7fffffffe5ff "/usr/local/bin/runc"
```

> 注意: 上方查看复杂结构体内容的方法.

## 解决方法

按照参考文章1的方法, 只参在进入`gdb`交互式命令行后, 才能设置参数.

```console
$ gdb runc
GNU gdb (GDB) Red Hat Enterprise Linux 7.6.1-120.el7
Copyright (C) 2013 Free Software Foundation, Inc.
...
(gdb) set args run container01
```

然后就可以进入期望的流程了.

同理, 可以通过`set environment KEY=VALUE`为目标程序设置环境变量.
