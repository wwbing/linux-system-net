#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int fd;
    pid_t pid;
    char buffer[100];
    const char *test_file = "test.txt";
    const char *message = "这是子进程写入的数据\n";
    
    // 在fork之前打开文件
    fd = open(test_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("打开文件失败");
        exit(1);
    }
    
    printf("父进程：打开文件成功，文件描述符为 %d\n", fd);
    
    // 创建子进程
    pid = fork();
    
    if (pid < 0) {
        perror("fork失败");
        close(fd);
        exit(1);
    } else if (pid == 0) {
        // 子进程
        printf("子进程：pid = %d，开始写入数据\n", getpid());
        
        // 子进程写入数据
        ssize_t write_bytes = write(fd, message, strlen(message));
        if (write_bytes < 0) {
            perror("子进程写入失败");
            exit(1);
        }
        
        printf("子进程：成功写入 %ld 字节数据\n", write_bytes);
        
        // 子进程退出
        exit(0);
    } else {
        // 父进程
        printf("父进程：等待子进程结束\n");
        
        // 等待子进程结束
        wait(NULL);
        
        printf("父进程：子进程已结束，开始读取文件\n");
        
        // 重置文件偏移量到文件开头
        lseek(fd, 0, SEEK_SET);
        
        // 父进程读取文件内容
        memset(buffer, 0, sizeof(buffer));
        ssize_t read_bytes = read(fd, buffer, sizeof(buffer) - 1);
        
        if (read_bytes < 0) {
            perror("父进程读取失败");
            close(fd);
            exit(1);
        }
        
        buffer[read_bytes] = '\0';
        printf("父进程：成功读取 %ld 字节数据\n", read_bytes);
        printf("父进程：读取的内容是：%s", buffer);
        
        // 验证读取的内容是否与子进程写入的内容一致
        if (strcmp(buffer, message) == 0) {
            printf("测试结果：父子进程共享文件描述符和文件偏移量\n");
        } else {
            printf("测试结果：父子进程不共享文件描述符或文件偏移量\n");
        }
        
        // 关闭文件
        close(fd);
    }
    
    return 0;
}
