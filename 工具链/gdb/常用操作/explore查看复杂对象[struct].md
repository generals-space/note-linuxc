# explore查看复杂对象[struct]

`p *结构体对象`, 可以查看目标结构的成员内容, 这里的'结构体对象'指的是这个指针哦, 所以前面会有'*'号

p命令还可以直接查看某一地址中的内容, 不过最好加上目标类型比如结构体类型, int等, 不然看到的是这个地址上本身的地址代码. 如

```c
p *(struct request *) 0x8b274c0
```

- `explore 结构体/对象`: 查看目标结构体的成员变量, 或是目标对象的成员值
- `explore type 结构体/对象`: 顾名思义, 可查看目标的类型及其成员类型
- `explore value 对象`: 无法直接查看未实例化的结构体的内容, 只能查看其实例化之后的结构体对象的所有成员的值
