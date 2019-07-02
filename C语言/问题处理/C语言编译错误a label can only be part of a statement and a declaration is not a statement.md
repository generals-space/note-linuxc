# C语言编译错误a label can only be part of a statement and a declaration is not a statement

```
a label can only be part of a statement and a declaration is not a statement
```

错误原因: 可能是在switch下的case语句中定义了变量并为其赋值;

解决办法: 将声明语句放在switch块外即可(或者给case语句加上大括号, 不过这个没试过).
