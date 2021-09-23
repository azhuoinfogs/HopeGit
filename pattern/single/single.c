
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
typedef struct _DATA
{
    void* pData;
    int i;
}DATA;
 
void* get_data()
{
    static DATA* pData = NULL;
    if(NULL != pData)
        return pData;
 
    pData = (DATA*)malloc(sizeof(DATA));
    assert(NULL != pData);
    pData->i = 1;
    return (void*)pData;
}
int main () {
    DATA *p = (DATA *)get_data();
    printf("%d\n", p->i);
    DATA *p1 = (DATA *)get_data();
    printf("%d\n", p1->i);

}