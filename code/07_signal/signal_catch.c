#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<signal.h>

void sig_catch(int signum){
	printf("\n捕捉函数启动\n");
	if(signum == SIGINT){
		printf("\n捕捉到信号%d\n",signum);
		sleep(5);
	}
	printf("\n捕捉函数结束\n");
}


int main(int argc, char* argv[]){

	struct sigaction act, oldact;
	act.sa_handler = sig_catch;
		
	sigemptyset(&(act.sa_mask));
	sigaddset(&act.sa_mask, SIGQUIT);

	act.sa_flags = 0;

	//注册信号捕捉函数
	int ret = sigaction(SIGINT, &act, &oldact);
	if(ret == -1){
		printf("注册捕捉函数失败\n");
	}

	while(1){
		printf("\n主函数进行中\n");
		sleep(1);
	}


	return 0;
}
