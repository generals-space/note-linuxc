# set与print-变量设置与查看

- `set $自定义变量名=某值`: 设置调试者自定义的变量;
- `p $自定义变量名`: 查看该变量的值;
- `set 源文件中变量名=某值`: 设置在调试过程中程序中的变量值;
    - 注意, 一定要在该变量的作用域范围内才行, 不然找不到
- `p 源文件中变量名`: 可用以查看变化;
