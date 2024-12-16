#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void sys_err(const char* str){
	perror(str);
	exit(1);
}


int main(){
	int i=0;

	for(i=0;i<5;++i){
		if(fork()==0){
			break;
		}
	}

	if(i==5){
		sleep(6);
		printf("我是父进程\n");
	}else{	
		sleep(i+1);
		printf("我是第%d个子进程\n",i);
	}
	
	
	return 0;
}