#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1001

int queue1[MAX],queue2[MAX],queue3[MAX],queue4[MAX];
int HEAD;

void FIFO(){
	
	int  head=HEAD, tail, n=1000, i, seek = 0, diff;
    float avg;							//平均寻道时间 
    tail = n;							//数组的末尾 
										//seek寻道总时间，diff磁道偏移量，head当前所处磁道号 
    for (i = 1; i <= n; i++)
    {
        diff = abs(head - queue1[i]);
        seek += diff;
        head = queue1[i];
    }

    avg = (float)seek / n;
    printf("FIFO总的寻道时间: %d\n", seek);
    printf("FIFO平均寻道时间: %f\n", avg);
}

void SSF(){
	
 	int head=HEAD, tail, n=1000, i, j, seek = 0, diff, min, pos;
    float avg;								//min用来找到最小磁道偏移量，pos记录当前最小磁道偏移量的位置 
    tail = n;
    for (i = 1; i <= n; i++)				
    {
        min = 9999;
        for (j = 1; j <= tail; j++)			//	找到对应的最小移动磁道
        {
            diff = abs(head - queue2[j]);
            if (diff < min)
            {
                min = diff;
                pos = j;
            }
        }
        diff = abs(head - queue2[pos]);
        seek += diff;
        head = queue2[pos];					//更新磁道号位置
        queue2[pos] = queue2[tail];			//将移除的磁道号用最末尾的代替，tail--
        tail--;
    }

    avg = (float)seek / n;
    printf("SSF总的寻道时间: %d\n", seek);
    printf("SSF平均寻道时间: %f\n", avg);
}

void SCAN(){
	
	int head=HEAD, tail, n=1000, i, j, seek = 0, diff, pos;
    float avg;
    tail = n;
    for (i = 1; i <= n; i++)
    {
        pos = -1;								
        diff = 9999;
        for (j = 1; j <= tail; j++)
        {
            if (queue3[j] >= head && abs(queue3[j] - head) < diff)	//找到离当前磁道号最近的位置
            {
                diff = abs(queue3[j] - head);						//排序可以，但是我没有用
                pos = j;
            }
        }
		
		//当前磁道号最大，处在最右边的情况 
        if (pos == -1)
        {
			int max=queue3[1];
			pos=1;
			for(int k=2;k<=tail;k++){				//寻找最大的磁盘号
				if(queue3[k] >max){
					pos=k;
					max=queue3[k];
				}
			}
       
        }
            diff = abs(queue3[pos] - head);
            seek += diff;
            head = queue3[pos];					//更新磁道号位置
            queue3[pos] = queue3[tail];			//将移除的磁道号用最末尾的代替，tail--
            tail--;
        
    }

    avg = (float)seek / n;
    printf("SCAN总的寻道时间: %d\n", seek);
    printf("SCAN平均寻道时间: %f\n", avg);
}

void CSCAN(){
	
	int head=HEAD, tail, n=1000, i, j, seek = 0, diff, pos;
    float avg;
    tail = n;

    for (i = 1; i <= n; i++)
    {
        pos = -1;
        diff = 9999;
        for (j = 1; j <= tail; j++)
        {
            if (queue4[j] >= head && abs(queue4[j] - head) < diff)
            {
                diff = abs(queue4[j] - head);
                pos = j;
            }
        }
    //当前磁道号最大，处在最右边的情况
        if (pos == -1)
        {
           int min=queue4[1];
			pos=1;
			for(int k=2;k<=tail;k++){				//寻找最小的磁盘号
				if(queue4[k]<min){
					pos=k;
					min=queue4[k];
				}
			}
        }
        	diff =abs(queue4[pos] - head);
            seek += diff;
            head = queue4[pos];
            queue4[pos] = queue4[tail];
            tail--;
        
    }
    avg = (float)seek / n;
    printf("CSCAN总的寻道时间: %d\n", seek);
    printf("CSCAN平均寻道时间: %f\n", avg);
	
}

int main()
{
	int i,a;
	srand(time(0)); /*初始化种子*/
	for( i = 1; i < 1001; i++ ) {
	a=rand()%100;				//0~99
	queue1[i]=a;		
	queue2[i]=a;
	queue3[i]=a;		
	queue4[i]=a;
}
	printf("输入刚开始的磁道号: ");
    scanf("%d", &HEAD);
	FIFO();
	SSF();
	SCAN(); 
	CSCAN(); 
    return 0;
}