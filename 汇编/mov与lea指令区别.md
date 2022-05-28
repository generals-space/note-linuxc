# mov与lea指令区别

参考文章

1. [汇编语言中mov和lea的区别有哪些？](https://www.zhihu.com/question/40720890)

```asm
	LEAQ	runtime·g0(SB), CX
	MOVQ	CX, g(BX)
	LEAQ	runtime·m0(SB), AX
	MOVQ	AX, m(BX)
```

lea是“load effective address”的缩写，简单的说，lea指令可以用来将一个内存地址直接赋给目的操作数，例如：

`lea eax,[ebx+8]`就是将ebx+8这个值直接赋给eax，而不是把ebx+8处的内存地址里的数据赋给eax。

而mov指令则恰恰相反，例如：

`mov eax,[ebx+8]`则是把内存地址为ebx+8处的数据赋给eax。

> 上面的汇编代码和参考文章1的解释是相反的, "leaq/movq src, dst", 而参考文章1的解释是 "lea/mov dst, src";