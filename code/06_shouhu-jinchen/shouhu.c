#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<sys/stat.h>
int main(){
	
	pid_t pid;
	pid = fork();

	if(pid>0){						//终止父进程
		exit(0);
	}
	
	pid = setsid();					 //子进程创建会话

	if(pid==-1){
		printf("会话创建失败\n");
	}

	int ret = chdir("/home/wwbing/cmd"); //改变工作目录
	
	umask(0);   					 //改变文件访问权限，传0代表777

	close(STDIN_FILENO);

	int fd = open("/dev/null",O_RDWR);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);

	return 0;
}
