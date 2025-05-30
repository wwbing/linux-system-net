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
	struct student stu;

	int fd = open("nb_temp",O_RDONLY);
	ftruncate(fd,sizeof(stu));

	struct student* map = mmap(NULL,sizeof(stu),PROT_READ,MAP_SHARED,fd,0);
	if(map == MAP_FAILED){
		printf("创建映射空间失败\n");
	}

	close(fd);
	
	while(1){
		printf("读取信息：学号：%f,姓名:%s,年龄:%d\n",map->id,map->name,map->age); //读映射空间	
		sleep(3);
	}
	
	int ret = munmap(map,sizeof(stu));
	if(ret==-1){
		printf("释放映射空间失败\n");
	}

	return 0;
}
