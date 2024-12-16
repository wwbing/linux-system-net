
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<signal.h>

void catch_child(int no_use){
	pid_t wpid;
	int status;
	while((wpid=waitpid(-1,&status,0))!=-1){
		if(status!=-1){
			printf("-------catch child %d \n",wpid);
		}		
	}
}

int main(){
	int i=0;
	pid_t pid;

	for(i=0;i<5;++i){
		if((pid=fork())==0){
			break;
		}
	}
	
	if(i==5){
		
		struct sigaction act;
		act.sa_handler = catch_child;

		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;

		sigaction(SIGCHLD,&act,NULL);
		printf("i am father,my pid is %d\n",getpid());
		
		while(1);
	}else{
		printf("i am %dth child, my pid is %d\n",i,getpid());
		return i;
	}

	return 0;
}
