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

	fd = open("fifo_p1",O_RDONLY); //只读打开fifo管道
	while(1){
		
		read(fd, buf, 4096);
		printf("%s",buf);
		sleep(3);
	}

	return 0;
}
