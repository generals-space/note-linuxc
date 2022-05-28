```
(gdb) bt
#0  startm (p=void, spinning=void) at /root/go/src/pkg/runtime/proc.c:1240
#1  0x000000000041475c in handoffp (p=void) at /root/go/src/pkg/runtime/proc.c:1269
#2  0x0000000000414853 in stoplockedm () at /root/go/src/pkg/runtime/proc.c:1365
#3  0x000000000041552b in runtime.gosched0 (gp=void) at /root/go/src/pkg/runtime/proc.c:1828
#4  0x000000000041a4c6 in runtime.newstack () at /root/go/src/pkg/runtime/stack.c:370
#5  0x0000000000420671 in runtime.morestack () at /root/go/src/pkg/runtime/asm_amd64.s:288
#6  0x00000000005704a8 in runtime.g0 ()
#7  0x000000000042050d in _rt0_go () at /root/go/src/pkg/runtime/asm_amd64.s:117
```

当前行运行在 startm() 函数中, 由1到7, 数值越大, 越处于上层.
