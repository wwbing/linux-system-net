#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

// 信号处理函数
void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("接收到 SIGINT 信号 (Ctrl+C)\n");
    } else if (signo == SIGTERM) {
        printf("接收到 SIGTERM 信号\n");
    } else if (signo == SIGUSR1) {
        printf("接收到 SIGUSR1 信号\n");
    } else if (signo == SIGUSR2) {
        printf("接收到 SIGUSR2 信号\n");
    } else {
        printf("接收到信号: %d\n", signo);
    }
}

int main() {
    // 注册信号处理函数
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
        perror("无法捕获 SIGINT");
        exit(EXIT_FAILURE);
    }
    
    if (signal(SIGTERM, sig_handler) == SIG_ERR) {
        perror("无法捕获 SIGTERM");
        exit(EXIT_FAILURE);
    }
    
    if (signal(SIGUSR1, sig_handler) == SIG_ERR) {
        perror("无法捕获 SIGUSR1");
        exit(EXIT_FAILURE);
    }
    
    if (signal(SIGUSR2, sig_handler) == SIG_ERR) {
        perror("无法捕获 SIGUSR2");
        exit(EXIT_FAILURE);
    }
    
    // 打印进程ID，方便发送信号
    printf("进程ID: %d\n", getpid());
    printf("等待信号...\n");
    printf("使用 Ctrl+C 发送 SIGINT\n");
    printf("使用 'kill -TERM %d' 发送 SIGTERM\n", getpid());
    printf("使用 'kill -USR1 %d' 发送 SIGUSR1\n", getpid());
    printf("使用 'kill -USR2 %d' 发送 SIGUSR2\n", getpid());
    
    // 无限循环等待信号
    while (1) {
        sleep(1);
    }
    
    return 0;
}
