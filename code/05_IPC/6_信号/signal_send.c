#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    int sig;
    
    // 检查命令行参数
    if (argc != 3) {
        printf("用法: %s <进程ID> <信号编号>\n", argv[0]);
        printf("例如: %s 1234 9\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // 获取进程ID和信号编号
    pid = atoi(argv[1]);
    sig = atoi(argv[2]);
    
    // 发送信号
    if (kill(pid, sig) == -1) {
        perror("发送信号失败");
        exit(EXIT_FAILURE);
    }
    
    printf("成功发送信号 %d 到进程 %d\n", sig, pid);
    
    return 0;
}
