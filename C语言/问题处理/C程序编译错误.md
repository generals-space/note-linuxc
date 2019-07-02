# C语言/C++编译错误

## 2.

```
error: expected constructor, destructor, or type conversion before '.' token.
```

光从这个错误提示是看不出什么问题的,所以不知道原因会感觉很奇怪. C++中, 全局域(在main函数之外, 与`#include`语句平级的地方)只能声明,初始化变量,不能对变量进行赋值,运算,调用函数等操作,谨记.

## 3.

```
relocation 0 has invalid symbol index
```

问题在于链接的时候找不到.o文件, 是不是不在同一目录又没显式指定路径?

## 5.

```
field has incomplete type
```

类或结构体的前向声明只能用来定义指针对象或引用，因为编译到这里时还没有发现定义，不知道该类或者结构的内部成员，没有办法具体的构造一个对象，所以会报错.

将类成员改成指针或引用就好了. 程序中使用incomplete type实现前向声明有助与实现数据的隐藏，要求调用对象的程序段只能使用声明对象的引用或者指针.

在显式声明异常规范的时候不能使用incomplete type.
