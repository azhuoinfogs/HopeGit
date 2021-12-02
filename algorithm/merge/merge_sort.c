/**
 * @file   code.c
 * @author  <kong@KONG-PC>
 * @date   Sun Nov 25 21:25:36 2012
 *
 * @brief  多路归并排序，这里是10路归并排序
 *
 *
 */
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

int a[1000000];                 /* 用来产生随机数 */
int b[100000];                  /* 用来暂存数据 */
char file_name[100][20];        /* 临时存储的文件名 */
FILE * file_fp[100];            /* 指向临时存储文件的指针 */

#define K 10                    /* K路归并排序 */
#define MIN -1                  /* 初始化败者树的时候做特殊值 */
#define MAX 100000000           /* 当某个临时文件读完时，叶子节点赋值
                                 * 为MAX，在程序中用来判断数据是否全部
                                 * 读取完毕 */

int loserTree[K];               /* 存储败者树中间节点值，下标0处存储冠军节点 */
int leaves[K+1];                /* 败者树的叶子节点从下标1开始存储叶子节点值，下标0处
                                 * 存储一个最小值节点 */
/**
 * 调整败者树
 *
 * @param i 需要调整的叶子节点的下标
 */
void adjust(int i)
{
    int parent=(i+K-1)/2;       /* 找到父节点 */
    while(parent>0)
    {
        if(leaves[i]>leaves[loserTree[parent]]) /* 和父节点比较，找到
                                                 * 新的优胜者 */
        {
            int temp=loserTree[parent];
            loserTree[parent]=i;
            /* i指向的是优胜者 */
            i= temp;
        }
        parent = parent / 2;
    }
    loserTree[0]=i;
}

void initLoserTree()
{
    leaves[0]=MIN;
    for(int i=0;i<K;i++)
        loserTree[i]=0;
    for(int i=K;i>0;i--)
        adjust(i);
}

void swap(int * a, int * b)
{
    int t= *a;
    *a=*b;
    *b=t;
}

int Rand(int begin, int end)
{
    int t = rand() % (end - begin) + begin;
    return t;
}

/**
 * 生成1000000个不重复的随机数
 *
 */
void rand_int()
{
    FILE * fp;
    fp = fopen("in", "w");
    assert(fp);
    srand(time(NULL));
    for(int i=0;i<1000000;i++)
        a[i]=i;
    for(int i=0;i<1000000;i++)
    {
        int t = Rand(i, 1000000);
        swap(&a[i],&a[t]);
    }
    for(int i=0;i<1000000;i++)
        fprintf(fp, "%d ", a[i]);
    fclose(fp);
}

int cmp(const void * m, const void * n)
{
    return *(int *)m - *(int *)n;
}

/**
 * 分K次读入文件，每次读入一部分数据，在内存中排完序之后，存入临时文件
 *
 */
void split_data()
{
    FILE * fp = fopen("in","r");
    assert(fp);
    for(int i=0;i<10;i++)
    {
        sprintf(file_name[i], "%s%d%s","data",i,".dat");
        file_fp[i] = fopen(file_name[i],"w");
        for(int j=0;j<100000;j++)
            if(fscanf(fp,"%d",&b[j]) == EOF)
                exit(1);

        /* 采用快速排序的方法 */
        qsort(b, 100000, sizeof(int), cmp);
        for(int j=0;j<100000;j++)
            if(fprintf(file_fp[i], "%d ", b[j]) == EOF)
                exit(1);
        fclose(file_fp[i]);
    }
    fclose(fp);
}

/** 
 * 将临时文件归并，结合败者树，每次从一列数中找出最小值的时候只需要
 * logn次比较,在时间要求不太明显的时候也可以遍历数组找出最值，比较次数
 * 为n
 *
 */
void merge_data()
{
    FILE * fp = fopen("out","w");
    int i;

    for(i=0;i<10;i++)
    {
        file_fp[i]=fopen(file_name[i],"r");
        assert(file_fp[i]);
    }

    /* 先从每个临时文件中读入一个数值，初始化叶子节点 */
    for(i=0;i<10;i++)
        if(fscanf(file_fp[i],"%d", &leaves[i+1])==EOF)
            leaves[i+1]=MAX;

    /* 构造败者树 */
    initLoserTree();

    while(1)
    {
        int flag = loserTree[0];
        int temp = leaves[flag]; /* 判断败者树的最小值，用来确定是否已
                                  * 经完全读取完毕 */
        if(temp==MAX)
            break;
        fprintf(fp, "%d ", temp);
        /* 输出最小值以后，再从响应的临时文件中读入一个数据，同时调整
         * 败者树 */
        if(fscanf(file_fp[flag-1], "%d", &leaves[flag]) == EOF)
            leaves[flag]=MAX;
        adjust(flag);
    }
    for(int i=0;i<10;i++)
        fclose(file_fp[i]);
    fclose(fp);
}


int main()
{
    rand_int();                 /* 产生随机数 */
    // split_data();               /* 将大文件分成多个分别有序的临时文件 */
    // merge_data();               /* 借助败者树将有序的临时文件归并在一起 */
    return 0;
}