#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

//兄弟进程通信实现 ls | wc -l
int main(int atgc, char* argv[]){
	
	pid_t pid;
	int fd[2]={0};	

	pipe(fd);

	int i;
	for(i=0;i<2;i++){
		if((pid =fork()) == 0){
			break; //保证子进程不创建孙进程
		}
	}

	if(i == 0){ // 儿子写
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		execlp("ls","ls",NULL);

	}else if(i == 1){		//兄弟读
		close(fd[1]);
		dup2(fd[0],STDIN_FILENO);
		execlp("wc","wc","-l",NULL);

	}else{		//父亲

		//为了保证管道数据的单向流通，必须关闭父亲进程的读写管道
		close(fd[1]);
		close(fd[0]);
		for(int i=0;i<2;++i){
			wait(NULL);
		}
	}
	

	return 0;
}
