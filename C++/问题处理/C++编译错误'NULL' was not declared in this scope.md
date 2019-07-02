# C++编译错误'NULL' was not declared in this scope

```
error: 'NULL' was not declared in this scope
```
NULL并不是关键字，在C++中貌似不能直接使用，但它被定义在一些标准头文件中，C++可以使用如下头文件：

```c
#include <cstddef>
```

这样还包括了一些其他基本类型如：`std::size_t`
