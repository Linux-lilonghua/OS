
//1. FIFO（先进先出）算法

#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int main()
{
    int queue[MAX], head, tail, n, i, seek = 0, diff;
    float avg;

    printf("请输入队列的尺寸: ");
    scanf("%d", &n);

    printf("输入队列的内容: ");			//对磁盘的访问时间最重要的时寻道时间，因此队列存取的内容为磁道号 
    for (i = 1; i <= n; i++)
        scanf("%d", &queue[i]);

    printf("输入刚开始的磁道号: ");
    scanf("%d", &head);

    tail = n;							//磁道的结束磁道号 

    for (i = 1; i <= n; i++)
    {
        diff = abs(head - queue[i]);
        seek += diff;
        head = queue[i];
    }

    avg = (float)seek / n;
    printf("总的寻道时间: %d\n", seek);
    printf("平均寻道时间: %f\n", avg);

    return 0;
}

//2. SSF（最短寻道时间优先）算法


#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int main()
{
    int queue[MAX], head, tail, n, i, j, seek = 0, diff, min, pos;
    float avg;

    printf("请输入队列的尺寸: ");
    scanf("%d", &n);

    printf("输入队列的内容: ");
    for (i = 1; i <= n; i++)
        scanf("%d", &queue[i]);

    printf("输入刚开始的磁道号: ");
    scanf("%d", &head);

    tail = n;

    for (i = 1; i <= n; i++)
    {
        min = 9999;
        for (j = 1; j <= tail; j++)			//	找到对应的最小移动磁道
        {
            diff = abs(head - queue[j]);
            if (diff < min)
            {
                min = diff;
                pos = j;
            }
        }
        diff = abs(head - queue[pos]);
        seek += diff;
        head = queue[pos];					//更新磁道号位置
        queue[pos] = queue[tail];			//将移除的磁道号用最末尾的代替，tail--
        tail--;
    }

    avg = (float)seek / n;
    printf("总的寻道时间: %d\n", seek);
    printf("平均寻道时间: %f\n", avg);

    return 0;
}

//3. SCAN（扫描）算法

#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int main()
{
    int queue[MAX], head, tail, n, i, j, seek = 0, diff, pos;
    float avg;

    printf("请输入队列的尺寸:");
    scanf("%d", &n);

    printf("输入队列的内容: ");
    for (i = 1; i <= n; i++)
        scanf("%d", &queue[i]);

    printf("输入刚开始的磁道号: ");
    scanf("%d", &head);

    tail = n;

    for (i = 1; i <= n; i++)
    {
        pos = -1;								
        diff = 9999;
        for (j = 1; j <= tail; j++)
        {
            if (queue[j] >= head && abs(queue[j] - head) < diff)	//找到相应的磁道号位置
            {
                diff = abs(queue[j] - head);						//排序可以，但是我没有用
                pos = j;
            }
        }
		
		//往回寻找
        if (pos == -1)
        {
			int max=queue[1];
			pos=1;
			for(int k=2;k<=tail;k++){				//寻找最大的磁盘号
				if(queue[k] >max){
					pos=k;
					max=queue[k];
				}
			}
       
        }
            diff = abs(queue[pos] - head);
            seek += diff;
            head = queue[pos];					//更新磁道号位置
            queue[pos] = queue[tail];			//将移除的磁道号用最末尾的代替，tail--
            tail--;
        
    }

    avg = (float)seek / n;
    printf("总的寻道时间: %d\n", seek);
    printf("平均寻道时间: %f\n", avg);

    return 0;
}

//4. C-SCAN（循环扫描）算法

#include <stdio.h>
#include <stdlib.h>

#define MAX 100

int main()
{
    int queue[MAX], head, tail, n, i, j, seek = 0, diff, pos;
    float avg;

    printf("请输入队列的尺寸: ");
    scanf("%d", &n);

    printf("输入队列的内容: ");
    for (i = 1; i <= n; i++)
        scanf("%d", &queue[i]);

    printf("输入刚开始的磁道号: ");
    scanf("%d", &head);

    tail = n;

    for (i = 1; i <= n; i++)
    {
        pos = -1;
        diff = 9999;
        for (j = 1; j <= tail; j++)
        {
            if (queue[j] >= head && abs(queue[j] - head) < diff)
            {
                diff = abs(queue[j] - head);
                pos = j;
            }
        }
        if (pos == -1)
        {
           int min=queue[1];
			pos=1;
			for(int k=2;k<=tail;k++){				//寻找最小的磁盘号
				if(queue[k]<min){
					pos=k;
					min=queue[k];
				}
			}
        }
			diff = abs(queue[pos] - head);
            seek += diff;
            head = queue[pos];
            queue[pos] = queue[tail];
            tail--;
        
    }

    avg = (float)seek / n;
    printf("总的寻道时间: %d\n", seek);
    printf("平均寻道时间: %f\n", avg);
	
    return 0;
}


//生成随机数
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
int a[1000];
int i;
srand(time(0)); /*初始化种子*/
for( i = 0; i < 1000; i++ ) 
a[i]=rand()%100;			//0~99;
for( i = 0; i < 1000; i++ ) 
printf("%d ", a[i]);
}