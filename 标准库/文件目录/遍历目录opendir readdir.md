# 遍历目录opendir readdir

参考文章

1. [Linux C/C++ ----目录文件的操作(opendir,readdir,closedir)](http://blog.chinaunix.net/uid-27213819-id-3810699.html)
2. [Linux C 遍历文件--opendir()、readdir()...上](https://blog.csdn.net/zyuseful/article/details/50428087)
    - 根据文件名过滤`.`与`..`两个目录

```c++
// dirent 库包含 opendir(), readdir() 函数
#include <dirent.h>
// 注意: strerror() 需要同时引入下面两个库, 否则printf()会出现段错误.
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main() {
    DIR* dir_fd;
    struct dirent* dir_item;
    dir_fd = opendir("/");
    if(dir_fd == NULL){
        printf("failed to open dir /: %s\n", strerror(errno));
        exit(1);
    }
    while((dir_item = readdir(dir_fd)) != NULL){
        // 过滤掉 readdir() 结果中的 . 与 .. 两个成员
        if(strcmp(dir_item->d_name, ".") == 0 || strcmp(dir_item->d_name, "..") == 0)
            continue;

        printf(dir_item->d_name);
        printf("\n");
    }
    closedir(dir_fd);

    return 0;
}
```

## 编译

```
gcc -g -o main main.c
```

## 运行

```
./main
```
