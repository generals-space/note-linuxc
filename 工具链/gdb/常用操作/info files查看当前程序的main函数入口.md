# info files查看当前程序的main函数入口

参考文章

1. [Go 1.5 源码剖析 （书签版）第2章 "引导"]()
    - gdb info files
2. [gdb深入学习手册](https://segmentfault.com/a/1190000022499197)

最近在看golang的源码分析, 版本为 1.2, 看到参考文章1处, 提到了gdb的`info files`指令.

感觉`info files`对于C语言之外的高级语言才有用, C语言本身倒没什么作用.

以如下C代码为例

```c++
#include <stdio.h>

int main() {
    printf("hello world!\n");
}
```

```
gcc -g -o main main.c
```

然后用gdb打开.

```console
$ gdb ./main
(gdb) info files
Symbols from "/root/main".
Local exec file:
	`/root/main', file type elf64-x86-64.
	Entry point: 0x400440
    ## ...省略
	0x0000000000400440 - 0x00000000004005c2 is .text
    ## ...省略
	0x0000000000601030 - 0x0000000000601034 is .data
	0x0000000000601034 - 0x0000000000601038 is .bss
(gdb) b *0x400440
Breakpoint 1 at 0x400440
(gdb) r
Starting program: /root/./main

Breakpoint 1, 0x0000000000400440 in _start ()
```

在执行r指令前, 向程序入口打断点, 就得可以得到汇编层面的入口源文件. 

不过, 这里得到的`_start ()`倒没显示, 应该是glibc等库里面的.

> 大型C程序工程中, 如果找不到main()函数入口, 也可以使用这种方式查看.

现在我们看看golang程序, 以go 1.2为例, 代码如下

```go
package main

func main(){
	println("hello world")
}
```

```
go build -gcflags='-l -N' -o main main.go 
```

然后用gdb打开.

```console
$ gdb ./main
(gdb) info file
Symbols from "/root/go/example/001.println/main".
Local exec file:
	`/root/go/example/001.println/main', file type elf64-x86-64.
	Entry point: 0x41bc20
	0x0000000000400c00 - 0x000000000041c08a is .text
    ## ...省略
	0x0000000000455040 - 0x00000000004596a8 is .data
	0x00000000004596c0 - 0x00000000004616b8 is .bss
	0x00000000004616c0 - 0x0000000000476c18 is .noptrbss
(gdb) b *0x41bc20
Breakpoint 1 at 0x41bc20: file /root/go/src/pkg/runtime/rt0_linux_amd64.s, line 8.
(gdb) l
1	package main
2
3	func main(){
4		println("hello world")
5	}
6
7	// go build -gcflags='-l -N' -o main main.go
(gdb) r
Starting program: /root/go/example/001.println/./main

Breakpoint 1, _rt0_amd64_linux () at /root/go/src/pkg/runtime/rt0_linux_amd64.s:8
```

这里就轻易得到了 golang runtime 的真正入口, 对于源码分析还是非常用用的.
