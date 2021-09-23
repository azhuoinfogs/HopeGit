#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef struct _DATA
{
    int i;
    struct _DATA* (*copy) (struct _DATA* pData);
}DATA;
struct _DATA* data_copy_A(struct _DATA* pData)
{
    DATA* pResult = (DATA*)malloc(sizeof(DATA));
    assert(NULL != pResult);
    memmove(pResult, pData, sizeof(DATA));
    return pResult;
};
DATA data_A = {2, data_copy_A};
struct _DATA* clone(struct _DATA* pData)
{
    return pData->copy(pData);
};
int main () {
    DATA *c = clone(&data_A);
    DATA *d = clone(&data_A);
    DATA *e = clone(&data_A);
    printf("%d--%d--%d\n", c->i, d->i, e->i);
}