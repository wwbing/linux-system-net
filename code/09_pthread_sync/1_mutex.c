#include<stdio.h>
#include<pthread.h>
#include<unistd.h>


//使用宏初始化也可，和pthread_mutex_init(&mutex,NULL)效果一样
pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;


void* pfunc1(void* arg){
    while(1){

        pthread_mutex_lock(&mutex);
        printf("hello ");
        
        printf("world\n");
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

void* pfunc2(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);
        printf("----------Hello ");

        printf("World--------\n");
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}


int main(){
    pthread_t tid1,tid2;
    //创建线程前初始化互斥锁。
    //pthread_mutex_init(&mutex,NULL);

    pthread_create(&tid1,NULL,(void *)pfunc1,NULL);
    pthread_create(&tid2,NULL,(void *)pfunc2,NULL);
    
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL); 
    pthread_mutex_destroy(&mutex);
    return 0;
}