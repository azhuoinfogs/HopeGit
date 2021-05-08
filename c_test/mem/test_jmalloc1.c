#include <stdlib.h>
#include <jemalloc/jemalloc.h>

void f()
{
    int i;
    for (i=0; i<1024*1024; ++i)
    {
        char *p = (char*)malloc(1024*1024*120);
        free(p);
    }
}

void fun1() {
  f();
}
void fun2() {
  f();
}

int main(int argc, char **argv )
{
    malloc(1000);
    // ProfilerStart("test.prof");//开启性能分析
    fun1();
    fun2();
    // ProfilerStop();//停止性能分析
    return 0;
}