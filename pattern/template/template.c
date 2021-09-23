#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
typedef struct _Basic
{
    void* pData;
    void (*step1) (struct _Basic* pBasic);
    void (*step2) (struct _Basic* pBasic);
    void (*process) (struct _Basic* pBasic);
}Basic;

void operate_of_step1(struct _Basic* pObject)
{
    assert(NULL != pObject);
    printf("operate_of_step1 node!\n");
}
void operate_of_step2(struct _Basic* pObject)
{
    assert(NULL != pObject);
    printf("operate_of_step2 node!\n");
}

void process(struct _Basic* pBasic)
{
    pBasic->step1(pBasic);
    pBasic->step2(pBasic);
}
int main() {
    Basic *parrent = malloc(sizeof (Basic));
    parrent->step1 = operate_of_step1;
    parrent->step2 = operate_of_step2;
    process(parrent);
}