#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>

int main(int argc, char* argv[]){
	
	alarm(1);

	int i=0;

	while(1){
		printf("%d\n",i++);	
	}

	return 0;
}
