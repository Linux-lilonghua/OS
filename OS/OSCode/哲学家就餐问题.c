
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5 // 哲学家数量
#define LEFT (i + N - 1) % N // 左邻居编号
#define RIGHT (i + 1) % N // 右邻居编号

sem_t mutex; // 互斥信号量
sem_t s[N]; // 哲学家信号量

void *philosopher(void *arg) {
    int i = *(int *)arg;
    while (1) {
        printf("Philosopher %d is thinking.\n", i);
        sleep(rand() % 5); 			// 随机思考一段时间
        sem_wait(&mutex); 			// 进入临界区
        sem_wait(&s[LEFT]); 		//如果没有获取到左叉子，阻塞自己
        sem_wait(&s[RIGHT]); 		//如果没有获取到右叉子，阻塞自己
        sem_post(&mutex); 			// 离开临界区
        printf("Philosopher %d is eating.\n", i);
        sleep(rand() % 5); 			// 随机就餐一段时间
        sem_post(&s[LEFT]); 		// 放下左边的叉子
        sem_post(&s[RIGHT]); 		// 放下右边的叉子
    }
}


int main() {
    pthread_t tid[N];
    int i;
    sem_init(&mutex, 0, 1); // 初始化互斥信号量
    for (i = 0; i < N; i++) {
        sem_init(&s[i], 0, 1); // 初始化哲学家信号量
    }
    for (i = 0; i < N; i++) {
        int *arg = (int *)malloc(sizeof(int));
        *arg = i;
        pthread_create(&tid[i], NULL, philosopher, arg); // 创建哲学家线程
    }
    for (i = 0; i < N; i++) {
        pthread_join(tid[i], NULL); // 等待哲学家线程结束
    }
    return 0;
}
