#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>

int add(int,int);
int sub(int,int);
int mul(int,int);

int main(int argc,char* argv[]){
	printf("%d+%d=%d\n",10,20,add(10,20));
	printf("%d-%d=%d\n",10,20,sub(10,20));
	printf("%d*%d=%d\n",10,20,mul(10,20));
	return 0;
}
