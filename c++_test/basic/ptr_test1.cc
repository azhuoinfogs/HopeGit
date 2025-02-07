#include <iostream>
#include <memory>

using namespace std;

class A;
class B;

class B
{
public:
	weak_ptr<A> aptr;
	B();
	~B();

private:

};

B::B()
{
	cout << "B()" << endl;
}

B::~B()
{
	cout << "B is deleted" << endl;
}

class A
{
public:
	weak_ptr<B> bptr;// 修改为weak_ptr
	A();
	~A();

private:

};

A::A()
{
	cout << "A()" << endl;
}

A::~A()
{
	cout << "A is deleted" << endl;
}


int main()
{
	shared_ptr<A> ap(new A);
	shared_ptr<B> bp(new B);

	ap->bptr = bp;
	bp->aptr = ap;

	cout << "main leave" << endl; // 循环引用导致ap bp退出了作用域都没有析构
	return 0;
}