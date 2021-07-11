# 流程控制-stepin next until return

单步运行时, 使用`s(stepin)`命令可以进入到函数内部. 与其相对的是`n(next)`跳过函数, 直接进行下一步.

`continue`: 单步调试期间, 可以动态设置断点, 设置后, 使用`continue`放开执行, 直到下一个断点.

`until 行号`: 跳到指定行, 可用于跳出循环

`return`: 可用于跳出当前函数(貌似`finish`不管用啊)

