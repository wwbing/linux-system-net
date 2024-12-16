#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>


int main(){
	char buf[10];
	int fd, n;

	fd = open("/dev/tty", O_RDONLY|O_NONBLOCK);
	if(fd<0){
		perror("open /dev/tty\n");
		exit(1);
	}

tryagain:
	n = read(fd,buf,10);
	if(n<0){
		if(errno != EAGAIN){
			perror("read /dev/tty\n");
			exit(1);
		}
		else{
			write(STDOUT_FILENO,"try again\n",strlen("try again\n"));
			sleep(2);
			goto tryagain;
		}
	}
	
	write(STDOUT_FILENO,buf,n);
	close(fd);

	return 0;
}
