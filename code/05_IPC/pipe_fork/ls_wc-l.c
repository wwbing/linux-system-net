#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>


//实现ls | ws -l
int main(){
	
	int fd[2]={0};
	pid_t pid;

	//创建进程和管道,一定要先创建管道，再创建进程。
	pipe(fd);
	
	//从fork之后的代码就要分父子进程，就会要拷贝内存中的数据信息
	//如果fork在pipe的前面，那么父子进程都会创建一个自己的fd文件符
	pid=fork();
	
	printf("%d,%d",fd[0],fd[1]);
	
	if(pid==0){	//儿子读
		close(fd[1]);
		dup2(fd[0],STDIN_FILENO);
		execlp("wc","wc","-l",NULL);

	
	}else if(pid > 0){  //父亲写
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		execlp("ls", "ls", NULL);
	}


	return 0;
}
