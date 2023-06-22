# 实验一、Windows 进程管理

## 1、实验题目

Windows 进程管理

## 2、实验目的

1. 学会使用VC 编写基本的 Win32 Consol Application（控制台应用程序)。
2. 通过创建进程、观察正在运行的进程和终止进程的程序设计和调试操作，进一步熟悉操作系统的进程概念，理解 Windows 进程的“一生”。
3. 通过阅读和分析实验程序，学习创建进程、观察进程、终止进程以及父子进程同步的基本程序设计方法。

## 3、实验内容

由于我平常用的都是Dev-C++来开发C/C++程序的，对于VC没怎么接触过，因此对于这次课设的验证性实验和设计类我都是用的Dev-C++来完成的。

### 3-1、编写基本的 Win32 Consol Application

这个实验的主要目的是为了熟悉编写C/C++控制台应用程序，对于创建一个C/C++控制台应用程序，我一般有俩种方式。
* 第一种，进入Dev-C++主界面，点击文件->新建->源代码。
* 第二种方式，点击文件->新建->项目->选择控制台应用程序。


![].\image\1.png)

前面是运行该程序所花费的时间，后面是程序的返回值，如果返回值为零，说明该程序运行正常，否则表示程序异常，或者非法访问等异常错误。

### 3-2、创建进程

* 用来创建进程的API函数CreateProcess()
```c
BOOL bCreateOK=::CreateProcess(
        NULL,						//子进程名称
        szCmdLine,					//命令行参数
        NULL,						// 不继承父进程的进程句柄
        NULL,						// 不进程父进程的线程句柄
        FALSE,						// 句柄继承标识为FALSE
        CREATE_NEW_CONSOLE,			// 使用新的控制台
        NULL,						// 使用父进程的环境
        NULL,						// 使用父进程的当前目录
        &si,						// 启动信息
        &pi) ;						// 返回的进程信息
```
* 用来获得当前进程标识符的API函数GetCurrentProcessId()
* 用来获得当前进程已加载的模块的API函数GetModuleFileName()
```c
// 提取用于当前可执行文件的文件名
    TCHAR szFilename[MAX_PATH] ;
    GetModuleFileName(NULL, szFilename, MAX_PATH) ;
```
* 进程终止的API函数ExitProcess() 或TerminateProcess()

拷贝实验给出的源代码到自己新建的工程下
1. 不修改的运行结果：会生成五个子进程

![](.\image\2.png)
    
在“命令提示符”窗口加入参数重新运行生成的可执行文件，会生成相应个数的子进程，如果输入的命令行参数大于1的话，那么所克隆出来的子进程为(5-命令行参数)

![](.\image\3.png)

2. 第一次修改nClone=0结果和源代码一样。
3. 第二次修改的运行结果会无限生成子进程，不管输入的命令行参数等于多少。
```c
  if (argc > 1)
    {
        // 从第二个参数中提取克隆ID
        :: sscanf(argv[1] , "%d" , &nClone) ; //用sscanf_s代替
    }
```
```c
nClone=0
```
原因在于：上述nclone语句是在if语句后面，导致父进程给子进程传递的参数不起作用，nclone一直为0。

### 3-3、父子进程的简单通信及终止进程

用于管理事件对象的 API

* CreateEvent()在内核中创建一个新的事件对象。
```c
HANDLE hMutexSuicide=CreateMutex(
        NULL,					// 缺省的安全性
        TRUE,					// 最初拥有的
        g_szMutexName) ;		// 互斥体名称
```
* OpenEvent()创建对已经存在的事件对象的引用。
```c
HANDLE hMutexSuicide = OpenMutex(
        SYNCHRONIZE,			// 打开用于同步
        FALSE,					// 不需要向下传递
        g_szMutexName) ;		// 名称
```
运行结果显示，不断克隆出子进程，这是因为程序路径名与命令行的参数没有空格分开，导致父进程给子进程传递的参数没有达到预期的效果。
```c
 sprintf(szCmdLine, "\"%s\"child" , szFilename)
```
改为：
```c
 sprintf(szCmdLine, "\"%s\" child" , szFilename)
```
运行结果显示：俩个进程，一个父进程，一个子进程，子进程通过接受父进程的信号而终止。

![](.\image\4.png)

```c
//父进程
ReleaseMutex(hMutexSuicide) 
```
```c
//子进程
WaitForSingleObject(hMutexSuicide, INFINITE) 
```
* 第一次修改，不断克隆出子进程。
* 第二次修改，子进程立即结束。
```c
WaitForSingleObject(hMutexSuicide, 0)
```
由于第二个参数为0，所以子进程并不会因为等待父进程的信号而阻塞，而是继续往下执行。

## 4、小结与心得体会

通过此时实验了解基本的控制台程序的创建和控制台程序传参的过程，通过简单的修改nClone的值，实现完全不同的效果，以及在Windows下如何创建进程，进程之间如何相互通信，如何实现互斥等操作。