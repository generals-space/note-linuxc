## make默认执行第一个标签的内容
all: init clear producer consumer
clean:
	rm -f *.o init clear producer consumer
init: common
	cc -g -o init sema_init.c sema_common.o
clear: common
	cc -g -o clear sema_clear.c sema_common.o
producer: common
	cc -g -o producer sema_producer.c sema_common.o
consumer: common
	cc -g -o consumer sema_consumer.c sema_common.o
common: ## 首先生成.h公共中间文件
	cc -g -c sema_common.c
