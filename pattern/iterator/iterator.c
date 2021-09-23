
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
typedef struct _Interator
{
    void* pVector;
    int index;
 
    int(* get_first)(struct _Interator* pInterator); 
    int(* get_last)(struct _Interator* pInterator);
}Interator; 

typedef struct _Container
{
    int* pData;
    int size;
    int length;
 
    Interator* (*create_new_interator)(struct _Container* pContainer);
    int (*get_first)(struct _Container* pContainer);
    int (*get_last)(struct _Container* pContainer);
 
}Container;

int vector_get_first(struct _Container* pContainer)
{
    assert(NULL != pContainer);
 
    return pContainer->pData[0];
}
 
int vector_get_last(struct _Container* pContainer)
{
    assert(NULL != pContainer);
   
    return pContainer->pData[pContainer->size -1];
}

struct _Interator* vector_interator_create(struct _Container* pContainer) {
    struct _Interator* pInterator = malloc (sizeof (struct _Interator));
    pInterator->index = 0;
    pInterator->pVector = pContainer;
    return pInterator;
}

int vector_interator_get_first(struct _Interator* pInterator)
{
    Container* pContainer;
    assert(NULL != pInterator && NULL != pInterator->pVector);
 
    pContainer = (struct _Container*) (pInterator->pVector);
    return pContainer ->get_first(pContainer);
}
 
int vector_interator_get_last(struct _Interator* pInterator)
{
    Container* pContainer;
    assert(NULL != pInterator && NULL != pInterator->pVector);
 
    pContainer = (struct _Container*) (pInterator->pVector);
    return pContainer ->get_last(pContainer);
}
int main () {
    Container *co = malloc (sizeof (Container));
    co->size = 10;
    co->length = 10;
    co->pData = malloc (co->size * sizeof (int));
    for (int i = 0; i < co->size; i++) {
        co->pData[i] = i;
    }
    co->create_new_interator = vector_interator_create;
    co->get_first = vector_get_first;
    co->get_last = vector_get_last;
    
    Interator * iter = vector_interator_create(co);
    int first = vector_interator_get_first(iter);
    printf("first-->%d\n", first);
    int last = vector_interator_get_last(iter);
    printf("last-->%d\n", last);
}