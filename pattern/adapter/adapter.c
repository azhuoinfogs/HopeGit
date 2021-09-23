#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 
typedef struct _Duck
{
    void (*fly)(struct _Duck* b);
    void (*quack)(struct _Duck* b);
}Duck;
 
typedef struct _DonaldDuck
{
    Duck duck;
    int donald_duck;
}DonaldDuck;
 
typedef struct _Turkey
{
    void (*fly)(struct _Turkey* b);
    void (*gobble)(struct _Turkey* b);
}Turkey;
 
typedef struct
{
    Turkey turkey;
    int wild_turkey;
}WildTurkey;
 
typedef struct _TurkeyAdapter
{
    Duck duck;
    Turkey* pTurkey;
    int turkey_adapter;
}TurkeyAdapter;
 
void donald_duck_fly(Duck* pDuck)
{
	printf("donald duck can fly long distance!\n");
	return;
}
 
void wild_turkey_fly(Turkey* pTurkey)
{
	printf("wild turkey can fly short distance!\n");
	return;
}
 
 
void turkey_adapter_fly(Duck* pDuck)
{
	TurkeyAdapter *pTurkeyAdapter = (TurkeyAdapter *)pDuck;
	pTurkeyAdapter->pTurkey->fly(pTurkeyAdapter->pTurkey);
}
 
void test_duck(Duck *pDuck)
{
	pDuck->fly(pDuck);
}
 
int main()
{
	Duck *pduck = NULL;
	DonaldDuck donald_duck;
	donald_duck.duck.fly = donald_duck_fly;
	pduck = (Duck *)&donald_duck;
	test_duck(pduck);
 
	WildTurkey wild_turkey;
	wild_turkey.turkey.fly = wild_turkey_fly;
 
	TurkeyAdapter adapter;
	adapter.pTurkey = (Turkey*)&wild_turkey;
	adapter.duck.fly = turkey_adapter_fly;
	pduck = (Duck *)&adapter;
	test_duck(pduck);
 
	return 0;
}