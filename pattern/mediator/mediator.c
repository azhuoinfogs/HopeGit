#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
typedef struct _Mediator Mediator;
typedef struct _People
{
    Mediator* pMediator; 
    void (*request)(struct _People* pPeople);
    void (*process)(struct _Peoplle* pPeople);
}People;

typedef struct _Mediator
{
    People* man;
    People* woman; 
}Mediator;

void man_request(struct _People* pPeople)
{
    assert(NULL != pPeople);
 
    pPeople->pMediator->woman->process(pPeople->pMediator->woman);   
}
void woman_request(struct _People* pPeople)
{
    assert(NULL != pPeople);
 
    pPeople->pMediator->man->process(pPeople->pMediator->man);
}
void man_process(struct _People* pPeople)
{
    assert(NULL != pPeople->pMediator->woman);
    printf ("receive woman message \n");
}
void woman_process(struct _People* pPeople)
{
    assert(NULL != pPeople->pMediator->woman);
    printf ("receive man message \n");
}

int main () {
    Mediator m;
    People man;
    man.pMediator = &m;
    man.process = man_process;
    man.request = man_request;
    People woman;
    woman.pMediator = &m;
    woman.process = woman_process;
    woman.request = woman_request;
    m.man = &man;
    m.woman = &woman;

    man_request(&man);
    woman_request(&woman);
}