#include <iostream>
class Target
{
public:
    virtual void Request() { std::cout << "Target::Request" << std::endl; }
};
//最终客户端要使用的类
class Adaptee
{
public:
    void SpecificRequest() { std::cout << "Adaptee::SpecificRequest" << std::endl; }
};
//  适配器类  转换接口类
class Adapter : public Target
{
public:
    Adapter() { _ade = new Adaptee(); }
    ~Adapter()
    {
        if (_ade != nullptr)
        {
            delete _ade;
            _ade = nullptr;
        }
    }
    //这样 虽然调用的是target接口 其实调用的是adaptee接口
    void Request() { _ade->SpecificRequest(); }
 
private:
    Adaptee *_ade;
};
// int main(int argc, char *argv[])
// {
//     Target *adt = new Adapter();
//     adt->Request();
//     return 0;
// }
using namespace std;
class VGA
{
public:
    virtual void play() = 0;
};
//VGA接口的投影仪
class TV1 : public VGA
{
public:
    void play()
    {
        cout<<"使用VGA接口的TV1播放视频"<<endl;
    }
};
class Computer
{
public:
    void playvideo(VGA *vag)
    {
        vag->play();
    }
    
};
int main()
{
    Computer c;
    c.playvideo(new TV1());
    return 0;
}
