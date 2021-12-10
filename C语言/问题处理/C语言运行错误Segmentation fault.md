# C语言运行错误Segmentation fault

```c++
#include <stdio.h> 
#include <string.h> 
int main(void) 
{ 
    char *buf; 　　　　　　　　　　　　　　//定义char指针
    char *string = "hello "; 　　　　　　//指向常量数据区的“hello”字符串
    buf = string; 　　　　　　　　　　　　　 //将指向常量的指针赋值

    printf("buf=%sn ", buf); 
    strcpy(buf, "world"); 　　　　　　　　　//试图将常量数据区的 "world"字符串拷贝给指向常量数据区的buf
    printf("buf=%sn", buf);
    return 0;
}
```

于是当运行时, 在strcpy处, 出现了段错误 `Segmentation fault`. 解决办法: 

1. buf没有空间, 应该用malloc分配空间

```
buf = malloc(4);
```

2. 改变 string为: 

```
char string[ ] = "hello";
```

这样, string是指向数组的指针, 赋值后, buf也是指向数组的指针, 再次调用strcpy时, 就把“world”复制到数组中了！

3. 可以直接赋值: 

```
buf = "world";
```

> 小结: 指针只存贮了一个地址, 想把整个字符串复制给他, 必须手动分配内存空间, 或存放于数组之中。(注意名词: **常量数据区**)