#include<stdio.h>
#include<unistd.h>
#include<pthread.h>


//三个线程的函数，实现三个不同终止线程的方法
void* pfunc1(void* arg){
    printf("func1: id is %lu \n",pthread_self());
    pthread_exit((void*)0); //退出当前线程
}

void* pfunc2(void* arg){
    printf("func2: id is %lu \n",pthread_self());
    return NULL;    //返回到调用者
} 


void* pfunc3(void* arg){

    printf("func3: id is %lu \n",pthread_self());
    pthread_cancel(pthread_self()); //直接终止当前线程，不等结束，外部调用也可以直接终止
}


int main(){

    pthread_t tid1,tid2,tid3;
    pthread_create(&tid1,NULL,(void*)pfunc1,NULL);
    //pth1不主动回收，那么在整个进程结束后，会被系统回收

    pthread_create(&tid2,NULL,(void*)pfunc2,NULL);
    pthread_detach(tid2);   //pth2结束后会自动回收

    pthread_create(&tid3,NULL,(void*)pfunc3,NULL);
    pthread_join(tid3,NULL); //pth3主动被主线程回收掉

    sleep(1);
    return 0;
}