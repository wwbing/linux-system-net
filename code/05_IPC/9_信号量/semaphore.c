#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

// 定义信号量操作的联合体
union semun {
    int val;               // SETVAL使用的值
    struct semid_ds *buf;  // IPC_STAT, IPC_SET使用的缓冲区
    unsigned short *array; // GETALL, SETALL使用的数组
};

// 初始化信号量
int init_sem(int sem_id, int value) {
    union semun sem_union;
    sem_union.val = value;
    
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) {
        perror("初始化信号量失败");
        return -1;
    }
    
    return 0;
}

// P操作（减少信号量，获取资源）
int sem_p(int sem_id) {
    struct sembuf sem_b;
    sem_b.sem_num = 0;     // 信号量编号
    sem_b.sem_op = -1;     // P操作
    sem_b.sem_flg = SEM_UNDO; // 进程终止时自动释放信号量
    
    if (semop(sem_id, &sem_b, 1) == -1) {
        perror("P操作失败");
        return -1;
    }
    
    return 0;
}

// V操作（增加信号量，释放资源）
int sem_v(int sem_id) {
    struct sembuf sem_b;
    sem_b.sem_num = 0;     // 信号量编号
    sem_b.sem_op = 1;      // V操作
    sem_b.sem_flg = SEM_UNDO; // 进程终止时自动释放信号量
    
    if (semop(sem_id, &sem_b, 1) == -1) {
        perror("V操作失败");
        return -1;
    }
    
    return 0;
}

// 删除信号量
void del_sem(int sem_id) {
    union semun sem_union;
    
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
        perror("删除信号量失败");
    }
}

// 临界区操作
void critical_section(int sem_id, int process_num) {
    // 获取信号量（P操作）
    printf("进程 %d 尝试进入临界区...\n", process_num);
    sem_p(sem_id);
    
    // 临界区
    printf("进程 %d 进入临界区\n", process_num);
    sleep(2);  // 模拟在临界区的操作
    printf("进程 %d 即将离开临界区\n", process_num);
    
    // 释放信号量（V操作）
    sem_v(sem_id);
    printf("进程 %d 已离开临界区\n", process_num);
}

int main() {
    int sem_id;
    pid_t pid;
    key_t key;
    
    // 生成唯一key
    key = ftok("/tmp", 'b');
    if (key == -1) {
        perror("ftok失败");
        exit(EXIT_FAILURE);
    }
    
    // 创建信号量
    sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("创建信号量失败");
        exit(EXIT_FAILURE);
    }
    
    // 初始化信号量为1（二进制信号量/互斥锁）
    if (init_sem(sem_id, 1) == -1) {
        exit(EXIT_FAILURE);
    }
    
    printf("信号量创建并初始化成功，ID: %d\n", sem_id);
    
    // 创建多个子进程
    for (int i = 1; i <= 3; i++) {
        pid = fork();
        
        if (pid < 0) {
            perror("创建子进程失败");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // 子进程
            critical_section(sem_id, i);
            exit(0);
        }
    }
    
    // 父进程等待所有子进程结束
    for (int i = 1; i <= 3; i++) {
        wait(NULL);
    }
    
    // 删除信号量
    del_sem(sem_id);
    printf("信号量已删除\n");
    
    return 0;
}
