#include <iostream>
#include <memory>
class D;
class B;
class C;
class A;
class D
{
private:
    std::shared_ptr<A> a_;

public:
    D(std::shared_ptr<A>& a): a_(a) {}
};
class A : public std::enable_shared_from_this<A>
{
private:
    std::weak_ptr<B> b_;
    std::weak_ptr<C> c_;
public:
    void do_something() {
        std::cout << "do_something" << std::endl;
    }
    void set_B_C(const std::shared_ptr<B>& b, const std::shared_ptr<C>& c)
    {
        b_ = b;
        c_ = c;
    }
    void new_D() {
        //错误方式，用this指针重新构造shared_ptr，将导致二次释放当前对象
        // std::shared_ptr<A> this_shared_ptr1(this);
        // std::unique_ptr<D> d1(new D(this_shared_ptr1));
        //正确方式
        std::shared_ptr<A> this_shared_ptr2 = shared_from_this();
        std::unique_ptr<D> d2(new D(this_shared_ptr2));
    }
};

class B
{
private:
    std::shared_ptr<A> a_;

public:
    B(std::shared_ptr<A>& a): a_(a) {}
    void do_something() {
        a_->do_something();
    }
};

class C
{
private:
    std::shared_ptr<A> a_;

public:
    C(std::shared_ptr<A>& a): a_(a) {}
    void do_something() {
        a_->do_something();
    }
};


std::shared_ptr<B> b_;
std::shared_ptr<C> c_;

//这个对象需要被多个 Class 同时使用的时候, 使用shared_ptr_
void test_A_B_C()
{
    std::shared_ptr<A> a = std::make_shared<A>();
    b_ = std::make_shared<B>(a);
    c_ = std::make_shared<C>(a);
    b_->do_something();
    c_->do_something();
    std::cout << a.use_count() << std::endl;
}

void test_A1_B_C()
{
    std::shared_ptr<A> a = std::make_shared<A>();
    b_ = std::make_shared<B>(a);
    c_ = std::make_shared<C>(a);

    // b_->do_something();
    // c_->do_something();
    a->set_B_C(b_, c_);
}
class TestUnique
{
private:
    std::unique_ptr<A> a_ = std::unique_ptr<A>(new A());
public:
    void process1()
    {
        a_->do_something();
    }

    void process2()
    {
        a_->do_something();
    }

    ~TestUnique()
    {
        //此处不再需要手动删除a_
    }
};
void incorrect_smart_pointer3()
{
    std::shared_ptr<A> shared_ptr_a1 = std::make_shared<A>();

    A *a= shared_ptr_a1.get();

    // std::shared_ptr<A> shared_ptr_a2(a);// 此处将导致对象的二次释放
    // A *a1= new A();
    // std::shared_ptr<A> shared_ptr_a2 = a1;//不允许
    std::shared_ptr<A> shared_ptr_a2 = shared_ptr_a1;

    // delete a;// 此处也将导致对象的二次释放
}
int main () {
    A a;
    a.do_something();
    std::unique_ptr<A> a_ptr1(new A());
    a_ptr1->do_something();
    
    // std::unique_ptr<A> a_ptr2 = a_ptr1;
    std::unique_ptr<A> a_ptr3 = std::move(a_ptr1);
    a_ptr3->do_something();
    std::unique_ptr<A> a_ptr4 = std::make_unique<A>();
    a_ptr4->do_something();
    std::cout << "=======================================" << std::endl;

    std::shared_ptr<A> b_ptr1(new A());
    std::shared_ptr<A> b_ptr2 = b_ptr1;//编译正常，允许所有权的共享
    b_ptr1->do_something();
    b_ptr2->do_something();
    std::cout << b_ptr1.use_count() << std::endl;
    std::cout << b_ptr2.use_count() << std::endl;
    b_ptr1.reset();
    std::cout << b_ptr1.use_count() << std::endl;
    std::cout << b_ptr2.use_count() << std::endl;
    std::cout << "=======================================" << std::endl;
    std::shared_ptr<A> c_ptr1(new A());
    std::weak_ptr<A> c_ptr2 = c_ptr1;//不增加引用计数
    std::cout << c_ptr1.use_count() << std::endl;
    std::cout << c_ptr2.use_count() << std::endl;
    std::cout << c_ptr2.expired()<< std::endl;
    c_ptr1.reset();
    std::cout << c_ptr2.expired()<< std::endl;
    if(std::shared_ptr<A> shared_a = c_ptr2.lock())
    {
        shared_a->do_something();
        //此时可以通过shared_a进行原生指针的方法调用
    }
    std::cout << "=======================================" << std::endl;
    test_A_B_C();
    std::cout << "=======================================" << std::endl;
    test_A1_B_C();
    std::cout << "=======================================" << std::endl;
    incorrect_smart_pointer3();
}