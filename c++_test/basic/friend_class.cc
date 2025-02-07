#include <iostream>
using namespace std;

class B;
class C;

class A {
private:
    int data;
    class C;
    
public:
    A(int value) : data(value) {}
    
    // 声明B类为友元类
    friend class B;
};

class B {
public:
    void showData(const A& obj) {
        // B类可以访问A类的私有成员
        cout << "A::data = " << obj.data << endl;
    }
};
class C {
public:
    void showData(const A& obj) {
        // B类可以访问A类的私有成员
        cout << "A::data = " << obj.data << endl;
    }
};

int main() {
    A objA(100);
    B objB;
    objB.showData(objA);  // 通过B类访问A类的私有成员
    return 0;
}
