/*name:fork.c
Function:创建两个进程, 子进程不断输出字符串, 父进程监听输入, 若用户输入x则同时退出
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

int main(){
	int var = 0;
	char msg;
	pid_t pid;
	pid = fork();
	if(pid < 0){
		printf("fork() error\n");
		exit(EXIT_FAILURE);
	}else if(pid == 0 ){
		while(1){
			var ++;
			printf("the child process %d say:\n", getpid());
			printf("%d\n", var);
			sleep(1);
		}
	}else{
		printf("i am the parent process %d\n",getpid());
		msg = getchar();
		if(msg == 'x'){
			kill(pid, SIGKILL);
			exit(EXIT_SUCCESS);
		}
	}
}
