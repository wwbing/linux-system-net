#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

void *p_fun(void *arg){
    int i = (int)arg;

    printf("i am %d th child pid is %d ,child thread id is: %lu\n",i,getpid(),pthread_self());

    return NULL;
}   

int main(){

    printf("I am main thread, pid is %d，thread_id is %lu \n",getpid(),pthread_self());
    pthread_t tid;

    for(int i=0;i<5;i++){
        int ret = pthread_create(&tid,NULL,p_fun,(void*)i);//创建子线程
    }
    //线程和当前的进程共享虚拟内存空间，如果当前进程直接结束，内存空间回收，那么线程也会结束
    //于是进程休眠一秒，给线程一点执行的时间

    sleep(1);   
    return 0;
}