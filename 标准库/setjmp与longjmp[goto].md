# setjmp与longjmp[goto]

参考文章

1. [C 语言中 setjmp 和 longjmp](https://www.cnblogs.com/hazir/p/c_setjmp_longjmp.html)
    - 不知怎么该语句在中国学生眼中就是臭名昭著, 几乎所有国内教材都一刀切地教大家尽量不要使用它
    - 但在我看来, 这根本不是语言的问题, 而是使用该语言的人, 看看 Linux 内核中遍地是`goto`语句的应用吧！

我们都知道要想在一个函数内进行跳转，可以使用 goto 语句，但如果从一个函数内跳转到另一个函数的某处，goto 是不能完成的，那该如何实现呢？

`setjmp(jmp_buf)`就点像游戏中的存档点, ta可以将其被调用的地方的上下文信息, 存储到`jmp_buf`结构体对象中;

之后可以使用`longjmp(jmp_buf)`进入指定的存档点, 即从该结构体中恢复.

需要注意的是, `longjmp(jmp_buf, val)`是不会返回的, 而是会跳进`setjmp()`方法, 借助ta来返回.

```c++
#include<setjmp.h>
#include<stdio.h>

void main(){
    jmp_buf env;
    int i;

    // 第1次执行 setjmp 设置存档点, 会返回0;
    // 第2次则由 longjmp 执行, 返回值为3;
    i = setjmp(env);
    printf("i = %d\n", i);
    if (i != 0 ) exit(0);

    // 进入 env 存档点, 并让该存档点的 setjmp() 返回 3.
    longjmp(env, 3);
    printf("不会运行到这里\n");
}
```

```
gcc -o setjmp ./setjmp.c
```

```console
$ ./setjmp
i = 0
i = 3
```
