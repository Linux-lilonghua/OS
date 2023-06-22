#### 1、关于c++中冒号::的用法

##### ::是运算符中等级最高的，它分为三种: 

###### 1、global scope(全局作用域符），用法（::name) 

###### 2、class scope(类作用域符），用法(class::name) 

###### 3、namespace scope(命名空间作用域符），用法(namespace::name) 他们都是左关联（left-associativity) 他们的作用都是为了更明确的调用你想要的变量

* 如在程序中的某一处你想调用全局变量a，那么就写成::a
* 如果想调用class A中的成员变量a，那么就写成A::a
* 另外一个如果想调用namespace std中的cout成员，你就写成std::cout（相当于using namespace std；cout）意思是在这里我想用cout对象是命名空间std中的cout（即就是标准库里边的cout）

#### 2、关于main(int argc,char* argv[])

##### 1、argc是命令行总的参数个数,argv[]是argc个参数，其中第0个参数是程序的全名，以后的参数命令行后面跟的用户输入的参数。

##### 2、char *argv[]是一个字符数组,其大小是int argc,主要用于命令行参数argv[]参数，数组里每个元素代表一个参数。
```c
#include <stdio.h>

int main(int argc,char* argv[])
{
        int i;
        printf("argc=%d\n\n",argc);
        for(i=0;i<argc;i++)
        {
                printf("argv[%d]=%s\n",i,argv[i]);
        }
        return 0;
}
```
输出结果为：
```shell
llh@llh-virtual-machine:~/demo$ ./a.out
argc=1

argv[0]=./a.out
```
```shell
llh@llh-virtual-machine:~/demo$ ./a.out 1 2 3 4 5
argc=6

argv[0]=./a.out
argv[1]=1
argv[2]=2
argv[3]=3
argv[4]=4
argv[5]=5
```
