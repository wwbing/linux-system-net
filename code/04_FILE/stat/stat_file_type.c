#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>
#include<pthread.h>
void sys_err(const char* str){
	perror(str);
	exit(1);

}

int main(int argc, char* argv[]){
	
	struct stat sbuf;

	int ret =stat(argv[1], &sbuf);
	
	if(ret==-1){
		sys_err("stat error\n");
	}

	if(S_ISREG(sbuf.st_mode)){
		printf("Is a regular file \n");
	}
	else if(S_ISDIR(sbuf.st_mode)){
		printf("Is a dir \n");	
	}

	else if(S_ISFIFO(sbuf.st_mode)){
		printf("Is a pipe \n");	
	}
	return 0;
}
