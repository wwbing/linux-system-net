#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>

int var = 100;

int main(int argc, char* argv[]){
	
	int *p;
	pid_t pid;

	int fd = open("temp", O_RDWR|O_CREAT|O_TRUNC,0664);
	if(fd<0){
		printf("映射文件打开失败\n");
	}
	ftruncate(fd, 4);
	
	p = (int*)mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(p==MAP_FAILED){
		printf("映射空间申请失败\n");	
	}
	close(fd);
	

	pid = fork();
	if(pid == 0){
		*p = 2000;	//写映射空间
		var = 10000;	//全局变量，读时共享写时复制，不会影响父进程
		printf("child = %d , var = %d\n", *p, var);
	}else if(pid > 0){	
		sleep(1);
		printf("parent = %d,var = %d\n",*p, var); //读映射空间
		wait(NULL);
		
		int ret = munmap(p,4);
		if(ret==-1){
			printf("映射空间释放失败！\n");
		}
	}


	return 0;
}
