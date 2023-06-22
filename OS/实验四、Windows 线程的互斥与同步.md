# 实验四、Windows 线程的互斥与同步

## 1、实验题目

Windows 线程的互斥与同步

## 2、实验目的

(1) 回顾操作系统进程、线程的有关概念，加深对 Windows 线程的理解。
(2) 了解互斥体对象，利用互斥与同步操作编写生产者-消费者问题的并发程序，加深对 P (即
semWait)、V(即 semSignal)原语以及利用 P、V 原语进行进程间同步与互斥操作的理解。

## 3、实验内容

根据实验的要求，写出相关的生产者消费者伪代码
```
Semaphore:
empty=n;			//empty表示当前缓冲区空闲的个数
full=0;				//full表示当前缓冲区产品的个数
mutex;				//mutux用来对访问缓冲区实施互斥
//生产者
Producer:
while(1){
P(empty)
P(mutex)
生产产品
将产品放到缓冲区中
V(mutex)
V(full)
}
//消费者
Consumer:
while(1){
P(full)
P(mutex)
从缓冲区中拿产品
消费产品
V(mutex)
V(empty)
}
```
首先了解一下基本信号量对象和互斥对象、线程如何创建的以及加锁解锁操作。
1. 创建或打开命名或未命名的信号量对象
```c++
HANDLE CreateSemaphore(
  LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,  // SD
  LONG lInitialCount,                          // initial count
  LONG lMaximumCount,                          // maximum count
  LPCTSTR lpName                           // object name
)
```
2. CreateMutex（）函数用来创建一个有名或无名的互斥量对象
```c++
 HANDLE CreateMutex( 
 LPSECURITY_ATTRIBUTES lpMutexAttributes,  // 指向安全属性的指针 
 BOOL bInitialOwner,  // 初始化互斥对象的所有者 
 LPCTSTR lpName  // 指向互斥对象名的指针 
 )
```
3. 创建线程
```c++
HANDLE CreateThread(
  LPSECURITY_ATTRIBUTES   lpThreadAttributes,
  SIZE_T                  dwStackSize,
  LPTHREAD_START_ROUTINE  lpStartAddress,
  __drv_aliasesMem LPVOID lpParameter,
  DWORD                   dwCreationFlags,
  LPDWORD                 lpThreadId
)
```
```c++
//加锁
WaitForSingleObject(hMutex, INFINITE)
//解锁
BOOL ReleaseMutex(HANDLE hSemaphore)
BOOL ReleaseSemaphore(
  [in]            HANDLE hSemaphore,
  [in]            LONG   lReleaseCount,
  [out, optional] LPLONG lpPreviousCount
)
```
了解了基本的知识之后接下来就是根据上述伪代码的逻辑具体的实现代码，实现也很简单，只需根据上述逻辑将上述的知识链接到里面即可。
运行结果：
* 信号量 EmptySemaphore 的初始化方法如果使得数量为零，则将导致全部进程都阻塞。
* 调整生产者线程和消费者线程的个数，当消费者数目大于生产者，消费者将经常等待生产者，反之，生产者经常等待消费者。

## 4、心得与体会

之前都是在Linux下实现生产者和消费者问题的，而现在是在Windows下实现，了解了Windows下如何创建进程和信号量和互斥量以及如何进行加锁和解锁操作，通过简单的修改，就会出现难以预料的结果，很有趣，学到很多知识。