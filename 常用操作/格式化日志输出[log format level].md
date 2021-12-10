# 格式化日志输出[log format level]

```c++
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

char* now()
{
    // 实例化time_t结构
    time_t now; 
    // 实例化tm结构指针
    struct tm *timenow;
    // time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
    time(&now);

    // localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)
    timenow = localtime(&now);

    // 上句中asctime函数把时间转换成字符，通过printf()函数输出
    // Sun Dec  5 16:49:19 2021
    // asctime(timenow))

    char* buf = malloc(20);
    strftime(buf, 20, "%Y-%m-%d %H:%M:%S", timenow);
    return buf;
}

#define PANIC   "PANIC"
#define FATAL   "FATAL"
#define ERROR   "ERROR"
#define WARNING "WARNING"
#define INFO    "INFO"
#define DEBUG   "DEBUG"

static void log_format(const char *level, const char *function, int line, const char *format, ...)
{
	char message[1024] = {};

    // va_XXX 系函数来自 stdarg.h
	va_list args;
	va_start(args, format);
	if (vsnprintf(message, sizeof(message), format, args) < 0)
		goto done;

	printf("%s %s %s:%d %s\n", now(), level, function, line, message);
done:
	va_end(args);
}

#define log_debug(fmt, ...) log_format(DEBUG, __FUNCTION__, __LINE__, (fmt), ##__VA_ARGS__)
#define log_info(fmt, ...) log_format(INFO, __FUNCTION__, __LINE__, (fmt), ##__VA_ARGS__)
#define log_warning(fmt, ...) log_format(WARNING, __FUNCTION__, __LINE__, (fmt), ##__VA_ARGS__)
#define log_error(fmt, ...) log_format(ERROR, __FUNCTION__, __LINE__, (fmt), ##__VA_ARGS__)
#define log_fatal(fmt, ...) log_format(FATAL, __FUNCTION__, __LINE__, (fmt), ##__VA_ARGS__)
#define log_panic(fmt, ...) log_format(PANIC, __FUNCTION__, __LINE__, (fmt), ##__VA_ARGS__)

#define BUFSIZE 1024

int main()
{
    log_info("hello world: %d %s", 12, "you");
    return 0;
}
```

```
2021-12-05 18:48:38 INFO main:70 hello world: 12 you
```
