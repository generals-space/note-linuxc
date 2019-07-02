# C++编译错误declare class does not name a type

```
declare class does not name a type
```

出现这个编译错误主要有四个可能原因，简单来说就是找不到目标类对象, 现总结如下：

1. 引用的类命名空间未包含
2. 引用的类头文件未包含
3. 包含了头文件，或者已经前向声明了，则说明所引用的类名写错。
4. 循环引用头文件

前置声明要素：

1. 前置声明需要注意以上提到的四点
2. 尽可能的采用前置声明（做到只有包含继承类的头文件）
3. 使用前置声明时，cpp文件中include 头文件次序必须先 包含前置声明的类定义头文件，再包含本类头文件。

否则会出现如下编译错误.

```
(expected constructor, destructor, or type conversion before ‘typedef')
```