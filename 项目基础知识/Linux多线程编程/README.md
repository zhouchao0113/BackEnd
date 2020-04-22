# Linux多线程编程

## 两个线程交替运行

### 互斥量版

```c++
/*
使用互斥量使两个线程严格交替运行
*/

#include <stdio.h>
#include <pthread.h>

pthread_t pid1,pid2;
pthread_mutex_t mutex1,mutex2;

void fun1()
{
    while (1)
    {
        //对mutex1加锁，下次运行需等到pthread2将mutex1解锁
        pthread_mutex_lock(&mutex1); 
        printf("this is thread 1\n");
        sleep(1);
        //对mutex2解锁，使得pthread2可以开始运行
        pthread_mutex_unlock(&mutex2);
    }
}

void fun2()
{
    while (1)
    {
        //对mutex2加锁，下次运行需等到pthread1将mutex2解锁
        pthread_mutex_lock(&mutex2);
        printf("this is thread 2\n");
        sleep(1);
        //对mutex1解锁，使得pthread1可以开始运行
        pthread_mutex_unlock(&mutex1);
    }
}

int main()
{
    pthread_mutex_init(&mutex1,NULL);
    pthread_mutex_init(&mutex2,NULL);

    //对mutex2加锁，保证pthread1先运行
    pthread_mutex_lock(&mutex2);

    pthread_create(&pid1,NULL,fun1,NULL);
    pthread_create(&pid2,NULL,fun2,NULL);

    pthread_join(pid1,NULL);
    pthread_join(pid2,NULL);

    return 0;
}
```

### [条件变量版](https://blog.csdn.net/hmxz2nn/article/details/80786188)

```c++
/*
利用条件变量实现两个线程交替打印
*/
#include <stdio.h>
#include <pthread.h>

pthread_t pid;
pthread_cond_t cond_main,cond_child;
pthread_mutex_t mutex;
int count = 0;

void fun()
{
    while (1)
    {
        //互斥锁是为了保护条件count
        pthread_mutex_lock(&mutex);
        //如果count为0时则阻塞等待cond_child条件的满足
        if (count==0)
        {
            //如果条件为假，线程通常会基于条件变量阻塞，并以原子方式释放等待条件变化的互斥锁。
            pthread_cond_wait(&cond_child,&mutex);
        }
        //将count设为0，下次本线程将被阻塞在cond_child条件上
        count=0;
        printf("this is child thread\n");
        sleep(1);
        //唤醒组设在cond_main条件上的线程
        pthread_cond_signal(&cond_main);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    //初始化条件变量与互斥锁
    pthread_cond_init(&cond_main,NULL);
    pthread_cond_init(&cond_child,NULL);
    pthread_mutex_init(&mutex,NULL);

    pthread_create(&pid,NULL,fun,NULL);

    while (1)
    {
        pthread_mutex_lock(&mutex);
        //如果count为1，则阻塞等待cond_main条件满足
        if (count==1)
        {
            pthread_cond_wait(&cond_main,&mutex);
        }
        //将count设为1，下次本线程将被阻塞在cond_main条件上
        count =1;
        printf("this is main thread\n");
        //唤醒阻塞在cond_child条件上的线程
        pthread_cond_signal(&cond_child);
        sleep(1);
        pthread_mutex_unlock(&mutex);
        
    }
    return 0;
}
```

