/**
银行家算法
*/
#include <stdio.h>
#include <stdlib.h>
#define M 4						//进程个数
#define N 3						//资源种类

//全局变量
int resource[N];		//总资源
int available[N];		//未分配的资源
int claim[M][N];		//进程的声明的最大资源
int alloc[M][N];		//进程已经分配的资源
int need[M][N];			//进程所需要的资源数量

int List[M];			//存放安全序列
int Work[N];			//未分配资源的代替
int finish[M];			//标记是否被加入到安全序列中

void init(){
    printf("%d种资源的总量：\n",N);						//资源种类的数量 
    for(int i=0;i<N;i++)
    scanf("%d",&resource[i]);
    printf("%d个进程所需要的最大资源数量：\n",M);		//声明进程所需要的最大资源数量 
    for(int i=0;i<M;i++)
        for(int j=0;j<N;j++)
        scanf("%d",&claim[i][j]);
    printf("%d个进程已经拥有的资源数量：\n",M);			//进程已经拥有的资源数量 
    for(int i=0;i<M;i++)
        for(int j=0;j<N;j++)
        scanf("%d",&alloc[i][j]);


        //计算进程所需的资源

    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            need[i][j]=claim[i][j]-alloc[i][j];
            }

    }
        //计算未分配的资源

    for(int i=0;i<N;i++){
        int sum=0;
        for(int j=0;j<M;j++){
             sum+=alloc[j][i];
        }
    available[i]=resource[i]-sum;
    }

}

//打印出进程资源分配表 

void printstate(){

printf("进程声明的最大资源Claim:\n");
for(int i=0;i<M;i++){
    for(int j=0;j<N;j++)
    printf("%d ",claim[i][j]);
    printf("\n");
}
printf("\n");

printf("进程已经拥有的资源Alloc:\n");
for(int i=0;i<M;i++){
    for(int j=0;j<N;j++)
    printf("%d ",alloc[i][j]);
    printf("\n");
}
printf("\n");

printf("need:\n");
for(int i=0;i<M;i++){
    for(int j=0;j<N;j++)
    printf("%d ",need[i][j]);
    printf("\n");
}
printf("\n");

printf("总资源Resource:\n");
for(int i=0;i<N;i++)
    printf("%d ",resource[i]);
    printf("\n\n");

printf("未分配的资源Available:\n");
for(int i=0;i<N;i++)
    printf("%d ",available[i]);
    printf("\n\n");

}


//银行家算法核心代码 
int safe(){						//判断是否为安全序列 
    int num=0;					//记录进程序列
    //初始化 
    for(int i=0;i<M;i++){		
        List[i]=0;
        finish[i]=0;
    }
    for(int i=0;i<N;i++){
        Work[i]=available[i];
    }
    
for(int k=0;k<M;k++){
  for(int i=0;i<M;i++){
    int count=0;			//记录是否满足资源请求
    if(finish[i]==0){
    //检查系统当前是否有足够资源满足该进程的请求
    for(int j=0;j<N;j++){
        if(need[i][j]<=Work[j])
            count++;
    }
    //找到满足要求的
    if(count==N){
            //将找到的放入序列中
            List[num++]=i;
            //标记这个被用过了
            finish[i]=1;
            //更新未分配的资源
    for(int l=0;l<N;l++){
        Work[l]+=alloc[i][l];
    }
    break;
                }
}
                    }
}
    if(num==M)return 1;
    else return -1;
}


int main()
{
init();			//初始化 
printstate();	//当前的进程资源分配 
if(safe()==1){
    printf("init is safe!\n");
     printf("安全序列：");
    for(int i=0;i<M;i++)
        printf("%d ",List[i]);
        printf("\n");
}else{
printf("init is not safe!\n");
return 0;
}

while(1){
int n,x,s,j;
int a=0,b=0,c=0;
printf("请输入要申请的进程号：");
scanf("%d",&n);
if(n<0||n>M){
    printf("输入的进程号不合法！\n");
}
else{
printf("请输入要申请的资源:");
scanf("%d%d%d",&x,&s,&j);
if(alloc[n][0]+x>claim[n][0])a=1;
if(alloc[n][1]+s>claim[n][1])b=1;
if(alloc[n][2]+j>claim[n][2])c=1;
if(a||b||c){
    printf("申请的资源不合法！\n");
}else{
alloc[n][0]+=x;
alloc[n][1]+=s;
alloc[n][2]+=j;
available[0]-=x;
available[1]-=s;
available[2]-=j;
for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            need[i][j]=claim[i][j]-alloc[i][j];
            }

    }
//银行算法
//判断是否为安全状态
if(safe()==1){
        //输出安全序列
        printf("它是安全状态\n");
        printf("安全序列：");
        for(int i=0;i<M;i++)
            printf("%d ",List[i]);
        printf("\n");
        printstate();
}
else{
    printf("它不是安全状态,拒绝分配资源\n");
    //恢复原状态
    alloc[n][0]-=x;
    alloc[n][1]-=s;
    alloc[n][2]-=j;
    available[0]+=x;
    available[1]+=s;
    available[2]+=j;
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            need[i][j]=claim[i][j]+alloc[i][j];
            }

    }
}
     }
  }
}
return 0;
}
/**
Claim:
3 2 2
6 1 3
3 1 4
4 2 2
Alloc:
1 0 0
5 1 1
2 1 1
0 0 2
Resource:
9 3 6
available:
0 1 1
p1申请R1和R3 不安全
p2申请R1和R3 安全
*/
