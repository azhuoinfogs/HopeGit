#include <stdio.h>
#include <stdlib.h>
int MergeSort_UptoDown(int *num, int start, int end);
int Merge(int *num, int start, int mid, int end);
int MergeSort(int *num, int start, int mid, int end ) {
    int *temp = (int *)calloc (end + 1, sizeof(int));
    int i = start;
    int k = 0;
    int j = mid + 1;
    while (1) {
        if (num[i] < num[j]) {
            temp[k] = num [i];
            i++;
            k++;
        }else {
            temp[k] = num [j];
            j++;
            k++;
        }
        if ((i > mid) || (j > end))
            break;
    }
    while(i <= mid) {
        temp[k]= num[i];
        i++;
        k++;
    }
    while(j <= end) {
        temp[k]= num[j];
        j++;
        k++;
    }
    for (i = 0; i < k; i++)
    {
        num[start + i] = temp[i];
    }
    free(temp);
    return 0;
}
int Merge(int *num, int start, int mid, int end)
{
    int *temp = (int *)malloc((end-start+1) * sizeof(int));    //申请空间来存放两个有序区归并后的临时区域
    int i = start;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= end)
    {
        if (num[i] <= num[j])
        {
            temp[k++] = num[i++];
        }
        else
        {
            temp[k++] = num[j++];
        }
    }

    while (i <= mid)
    {
        temp[k++] = num[i++];
    }
    while (j <= end)
    {
        temp[k++] = num[j++];
    }

    //将临时区域中排序后的元素，整合到原数组中
    for (i = 0; i < k; i++)
    {
        num[start + i] = temp[i];
    }

    free(temp);
    return 0;
}
int MergeSort_UptoDown(int *num, int start, int end){
    int mid = start + (end - start) / 2;
    if (start >= end)
    {
        return;
    }
    MergeSort_UptoDown(num, start, mid);
    MergeSort_UptoDown(num, mid + 1, end);
    MergeSort(num,start, mid, end);
    return 0;
}
int main()
{
    /* 归并排序（升序） */
    int num[10] = {5, 1, 8, 4, 7, 2, 3, 9, 0, 6};
    int length = sizeof(num) / sizeof(num[0]);
    int i;

    MergeSort_UptoDown(num, 0, length - 1);

    for (i = 0; i < length; i++)
    {
        printf("%d ", num[i]);
    }

    return 0;
}