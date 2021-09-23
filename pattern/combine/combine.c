#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef struct _Object
{
    struct _Object** ppObject;
    int number;
    void (*operate)(struct _Object* pObject);
}Object;

void operate_of_parent(struct _Object* pObject)
{
    printf("parent node!\n");
    int index;
    assert(NULL != pObject);
    assert(NULL != pObject->ppObject && 0 != pObject->number);
    for(index = 0; index < pObject->number; index ++)
    {
        pObject->ppObject[index]->operate(pObject->ppObject[index]);
    }
}
void operate_of_child(struct _Object* pObject)
{
    assert(NULL != pObject);
    printf("child node!\n");
}
void process(struct _Object* pObject)
{
    assert(NULL != pObject);
    pObject->operate(pObject);
}
int main() {
    Object *parrent = malloc(sizeof (struct _Object));
    parrent->number = 2;
    parrent->ppObject = malloc( parrent->number * sizeof (struct _Object));
    int index;
    for(index = 0; index < parrent->number; index ++) {
        parrent->ppObject[index] =(struct _Object*) malloc(sizeof (struct _Object));
        parrent->ppObject[index]->number = 0;
        parrent->ppObject[index]->operate = operate_of_child;
        parrent->ppObject[index]->ppObject=  NULL;
    }
    parrent->operate = operate_of_parent;
    // process(parrent);
    process(parrent->ppObject[0]);
}
