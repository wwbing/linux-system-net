#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<signal.h>

void print_pedset(sigset_t *set){
	int i=0;
	for(i=1;i<32;++i){
		if(sigismember(set,i)){
			putchar('1');
		}
		else{
			putchar('0');
		}
	}
	printf("\n");
}


int main(int argc, char* argv[]){
	
	sigset_t set, old_set, pedset;
	
	//创建自定义信号集
	sigemptyset(&set);
	
	//把ctrl + c 中断信号加入自定义集中
	sigaddset(&set,SIGINT);
	
	sigaddset(&set,SIGKILL);
	sigaddset(&set,SIGSTOP);
	

	//根绝自定义信号集 设置阻塞信号集信号阻塞状态
	int ret = sigprocmask(SIG_BLOCK, &set, &old_set);

	if(ret == -1){
		printf("借助自定义信号集设置SIGINT为阻塞状态失败\n");
	}
	
	printf("当前进程的pid：%d",getpid());

	//打印未决信号集
	while(1){
		ret = sigpending(&pedset);
		print_pedset(&pedset);
		sleep(1);	
	}

	return 0;
}
