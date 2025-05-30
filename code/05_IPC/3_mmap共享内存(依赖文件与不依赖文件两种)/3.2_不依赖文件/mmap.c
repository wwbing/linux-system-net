#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>

int main(int argc, char* argv[]){
	
	//不需要创建文件来创建映射空间
	//第四个参数映射空间属性添加上：MAP_ANONYMOUS便可
	//文件大小任意指定，文件描述符位置填-1
	char* map = mmap(NULL,4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

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
