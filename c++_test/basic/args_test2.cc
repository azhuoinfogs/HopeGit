#include <iostream>
using namespace std;


class classA {
public:
    classA() { cout << "classA()" << endl; }
    classA(const classA& a) { cout << "copy classA()" << endl; }
    ~classA() { cout << "~classA()" << endl; }
    classA& operator=(const classA& a) {
        cout << "operator=" << endl;
        return *this;
    }
};

class classB
{
public:
    classB(classA a)  {mA = a;}//先调用a 的默认构造函数在进行赋值
	~classB() { cout << "~classB()" << endl; }
private:
    classA mA;
};


int main()
{
    classA a;
    classB b(a);
    // classA() 
// copy classA()
// classA() ：classB中调用默认构造函数构造mA；
// operator=
// ~classA()
// ~classB()
// ~classA()
// ~classA()
}
