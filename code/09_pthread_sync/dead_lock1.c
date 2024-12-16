#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

pthread_mutex_t m1;
char val1[100];


void* pfunc1(void* arg){
    char* p = (char*)arg;
    while(1){
        pthread_mutex_lock(&m1); 
        pthread_mutex_lock(&m1);    //再次加锁，自己阻塞自己，没法解锁
        strcpy(val1, p);
        printf("%s\n",val1);
        
        pthread_mutex_unlock(&m1);


        sleep(1);
    }
}



int main(){

    pthread_t tid1;
    pthread_mutex_init(&m1,NULL);

    pthread_create(&tid1,NULL,pfunc1,(void*)"func1");

    pthread_join(tid1,NULL);

    return 0;
}