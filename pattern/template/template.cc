#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;
class basic
{

public:
    basic() {}
    virtual ~basic() {}
    virtual void step1() {}
    virtual void step2() {}
    void process()
    {
        step1();
        step2();
    }
};
class data_A : public basic
{
public:
    data_A() {}
    ~data_A() {}
    void step1() 
    {
        printf("step 1 in data_A!\n");
    }
 
    void step2()
    {
        printf("step 2 in data_A!\n");
    }
};
int main () {
    data_A *a = new data_A;
    a->process();
}