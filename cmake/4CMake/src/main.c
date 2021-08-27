#include <stdio.h>
#include "testFunc.h"
#include "testFunc1.h"

int main(void)
{
#ifdef WWW1
	func(100);
    printf("hello world1\n");
#endif    

#ifdef WWW2
	func1(200);
    printf("hello world2\n");
#endif
	return 0;
}