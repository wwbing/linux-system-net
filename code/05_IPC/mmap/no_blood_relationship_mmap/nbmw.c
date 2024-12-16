#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>

struct student{
	double id;
	char name[20];
	int age;
};


int main(int argc, char* argv[]){
	struct student stu = {2315363128, "jiahao", 24};

	int fd = open("nb_temp",O_RDWR|O_CREAT|O_TRUNC, 0664);
	ftruncate(fd,sizeof(stu));

	struct student* map = mmap(NULL,sizeof(stu),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(map == MAP_FAILED){
		printf("创建映射空间失败\n");
	}

	close(fd);
	
	while(1){
		memcpy(map,&stu,sizeof(stu)); //写映射空间
		stu.age++;
		printf("写入\n");
		sleep(1);
	}
	
	int ret = munmap(map,sizeof(stu));
	if(ret==-1){
		printf("释放映射空间失败\n");
	}

	return 0;
}
