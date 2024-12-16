#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

char buf[1024];


int main(int argc, char* argv[]){
	
	int src, dst;
	int n;

	//只读打开源文件
	src = open(argv[1], O_RDONLY);
	printf("源文件只读打开成功,编号：%d\n",src);
	
	//尝试只写打开目标文件，不存在就创建后再只读打开
	dst = open(argv[2], O_WRONLY,0777);
	if(dst==-1){
		dst = open(argv[2],O_CREAT, 0777);
		printf("目标文件创建成功\n");
		dst = open(argv[2], O_WRONLY,0777);
		printf("目标文件只写打开成功,编号:%d\n",dst);

	}else{
		printf("目标文件直接只写打开成功,编号:%d\n",dst);	
	}

	//读取源文件的内容到缓冲区
	//把缓冲区的内容写入目标文件
	while((n=read(src,buf,1024))>0){
		int wn = write(dst, buf, n);
		printf("写入成功:%d字节\n",wn);
	}

	//关闭源文件
	close(src);


	//关闭目标文件
	close(dst);




	return 0;
}

