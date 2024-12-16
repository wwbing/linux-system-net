#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>

int main(int argc, char* argv[]){
	
	int fd = open("test.mmap",O_RDWR|O_CREAT|O_TRUNC,0664);
	ftruncate(fd, 4);

	char* map = mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	close(fd);  //根据文件创建了映射就可以关闭文件了，后续根据映射的地址去访问文件

	if(map==MAP_FAILED){
		printf("申请映射地址空间错误\n");
	}

	strcpy(map,"abaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaacdefgh"); //通过map写入文件中
	printf("%s\n",map); //读文件中

	if(munmap(map,4)!=-1){
		printf("释放映射地址空间成功!\n");
	}


	return 0;
}
