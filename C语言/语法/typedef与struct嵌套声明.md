# typedef与struct嵌套声明

参考文章

1. [结构体的嵌套 自身嵌套 相互嵌套](https://blog.csdn.net/weixin_42167759/article/details/80330953)

```c
typedef struct rbt_root_t{
    rbt_t* root;
    rbt_t* nil;
}rbt_root_t;
```

但是在编译的时候会出错: 错误：未知的类型名‘rbt_root_t’

gcc版本: 4.8.5 20150623 (Red Hat 4.8.5-39) (GCC)

然后按照参考文章1, 改成了

```c
typedef struct rbt_root_t rbt_root_t;
struct rbt_root_t{
    rbt_t* root;
    rbt_t* nil;
};
```

就可以了.

