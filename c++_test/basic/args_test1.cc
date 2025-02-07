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
    classB(classA a) : mA(a) {}
	~classB() { cout << "~classB()" << endl; }
private:
    classA mA;
};


int main()
{
    classA a;
    classB b(a);
    classA()
    // copy classA()
    // copy classA()
    // ~classA()
    // ~classB()
    // ~classA()
    // ~classA()
    // 第一个 ~classA() 输出是由于临时对象（传递给 classB 构造函数的那个）的析构。
}
