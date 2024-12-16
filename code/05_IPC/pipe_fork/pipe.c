#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(){
	
	int fd[2]={0};
	pid_t pid;

	if(pipe(fd)<0){  //创建管道 。创建文件描述符号保存到fd数组
		printf("管道创建失败!\n");
	}

	pid = fork(); //创建进程

	if(pid==0){		//子进程读
		close(fd[1]); 
		sleep(1); //等待父亲写完
		
		char buf[50]={0};
		read(fd[0],buf,sizeof(buf));

		printf("子读取成功 %s\n",buf);
		printf("子进程结束\n");

	}else if(pid>0){	//父进程写

		close(fd[0]);
		char buf[] = "i am message ";
		write(fd[1],buf,strlen(buf));

		printf("父写入成功 %s\n",buf);
		printf("父等待回收子进程中........ \n");
		
		wait(NULL); //等待子结束好回收
		printf("父亲回收成功，父亲结束\n");
	
	}else{
		printf("进程创建失败\n");
		exit(1);
	}

	return 0;
}
