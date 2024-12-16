#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>


int main(){
	
	int fd,i;
	char buf[4096];

	mkfifo("fifo_p1", 0644); //创建fifo，指定管道名和八进制权限
	fd = open("fifo_p1",O_WRONLY); //只写打开fifo管道
	while(1){
		sprintf(buf, "hello jiahao %d\n",i++);
		printf("%s",buf);
		write(fd, buf, strlen(buf));
		sleep(1);
	}

	return 0;
}
