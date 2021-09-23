#include<string.h>
#include<iostream>
using namespace std;
class Resume
{
protected:
	char *name;
public:
	Resume() {}
	virtual ~Resume() {}
	virtual Resume* Clone() { return NULL; }
	virtual void Set(char *n) {}
	virtual void Show() {}
};
 
class ResumeA : public Resume
{
public:
	ResumeA(const char *str);  //构造函数
	ResumeA(const ResumeA &r); //拷贝构造函数
	~ResumeA();                //析构函数
	ResumeA* Clone();          //克隆，关键所在
	void Show();               //显示内容
};
ResumeA::ResumeA(const char *str) 
{
	if(str == NULL) {
		name = new char[1]; 
		name[0] = '\0'; 
	}
	else {
		name = new char[strlen(str)+1];
		strcpy(name, str);
	}
}
ResumeA::~ResumeA() { delete [] name;}
ResumeA::ResumeA(const ResumeA &r) {
	name = new char[strlen(r.name)+1];
	strcpy(name, r.name);
}
ResumeA* ResumeA::Clone() {
	return new ResumeA(*this);
}
void ResumeA::Show() {
	cout<<"ResumeA name : "<<name<<endl; 
}

int main () {
    Resume *r1 = new ResumeA("A");
	Resume *r2 = r1->Clone();
	Resume *r3 = r1->Clone();
	Resume *r4 = r1->Clone();
	Resume *r5 = r1->Clone();
	Resume *r6 = r1->Clone();
	r1->Show();
	//删除r1,r2
	delete r1;	
	r1 = NULL;
	//深拷贝所以对r3,r4无影响
	r3->Show();
	delete r3;
	r3 = NULL;
}
