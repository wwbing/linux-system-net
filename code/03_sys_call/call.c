#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

int main(int argc, char *argv[]){
	
	//打开文件
	int fd = open("./test.out", O_CREAT,0664);
	printf("文件描述符:%d\n",fd);
	
	//文件操作



	//关闭文件
	int ret = close(fd);
	printf("close 返回:%d\n",ret);
	return 0;
}
