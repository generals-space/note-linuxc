# 汇编指令[disas si ni]

参考文章

1. [GDB 常用命令学习](https://blog.csdn.net/The_sun0808/article/details/50000447)
    - disas, si(汇编级别步入指令, 同"step in"), ni(汇编级别步进指令, 同"step next")
2. [Can I force GDB breakpoints and disassemble?](https://reverseengineering.stackexchange.com/questions/25616/can-i-force-gdb-breakpoints-and-disassemble)
    - 在汇编级别源码中打断点.

disas: disassemble

si命令类似于s命令，ni命令类似于n命令。所不同的是，这两个命令（si/ni）所针对的是汇编指令，而s/n针对的是源代码。

```
(gdb) disass
Dump of assembler code for function main.main:
=> 0x0000000000400c00 <+0>:	mov    %fs:0xfffffffffffffff0,%rcx
   0x0000000000400c09 <+9>:	cmp    (%rcx),%rsp
   0x0000000000400c0c <+12>:	ja     0x400c15 <main.main+21>
   0x0000000000400c0e <+14>:	callq  0x41b290 <runtime.morestack00>
   0x0000000000400c13 <+19>:	jmp    0x400c00 <main.main>
   0x0000000000400c15 <+21>:	callq  0x400c00 <main.main>
   0x0000000000400c1a <+26>:	retq
```

如果我们想在汇编级别的语句中, 添加断点的话, 可以使用"b *0xFFFFFFFFF"这种格式, 如

```
(gdb) b *0x0000000000400c0e
Breakpoint 3 at 0x400c0e: file /root/go/gopath/src/005.split.stack/./main.go, line 3.
```

然后执行"c"指令, 就可以触发到了.
