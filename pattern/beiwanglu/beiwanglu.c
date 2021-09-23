#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
void process(void* pData) {
}
typedef struct _Action
{
    int type;
    struct _Action* next;
    void* pData;
    void (*process)(void* pData);
}Action;

typedef struct _Organizer
{
    int number;
    Action* pActionHead;
    Action* (*create)();
    void (*restore)(struct _Organizer* pOrganizer); 
}Organizer;


typedef struct _Caretake    
{
    void (*Save)(void* pData);
    Action (*Load)(int state);
    Action action[10];
    int index;
}Caretake;
void Save(void *pData) {
    
}

void restore(struct _Organizer* pOrganizer)
{
    Action* pHead;
    assert(NULL != pOrganizer);
 
    pHead = pOrganizer->pActionHead;
    pHead->process(pHead->pData);
    pOrganizer->pActionHead = pHead->next;
    pOrganizer->number --;
    free(pHead);
    return;
}
Action *create()
{
    Action *action = malloc (sizeof(Action));
    action->type = 0;
    action->next = NULL;
    action->pData = malloc (10);
    action->process = NULL;
    return action;
}
int main () {
    Caretake *take = malloc (sizeof (Caretake));
    take->Save = 
    Organizer *orgnize = malloc (sizeof(Organizer));
    memset(orgnize, 0, sizeof (Organizer));
    orgnize->create = create;
    orgnize->number = 1;
    orgnize->restore = restore;
}