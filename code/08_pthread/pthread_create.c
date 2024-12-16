#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<errno.h>
#include<sys/stat.h>

void p_fun(void){
    printf("thread create\n");
    
}

//使用线程，gcc编译时加上-lpthread参数：gcc pthread.c -lpthread
int main(){

    pthread_t tid;

    int ret = pthread_create(&tid, NULL, (void*)p_fun, NULL);

    sleep(1);

    return 0;
}