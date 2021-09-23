#include <iostream>
using namespace std;
 
typedef int Object ;
#define SIZE 5 
 
//注意类的顺序 
class MyIterator
{
public:
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual bool IsDone() = 0;
	virtual Object CurrentItem() = 0;
};
 
class Aggregate
{
public:
	virtual Object getItem(int index) = 0;
	virtual MyIterator *CreateIterator() = 0;
	virtual int getSize() = 0;
};
 
class ContreteIterator : public MyIterator
{
public:
	ContreteIterator(Aggregate *ag) 
	{
		_ag = ag;
		_idx = 0;
	}
	~ContreteIterator()
	{
		_ag = NULL;
		_idx = 0;
	}
 
	virtual void First()
	{
		_idx = 0;
	}
	virtual void Next()
	{
		if (_idx <	_ag->getSize())
		{
			_idx ++;
		}
	}
	virtual bool IsDone()
	{
		return (_idx == _ag->getSize());
	}
	virtual Object CurrentItem()
	{
		return _ag->getItem(_idx);
	}
 
protected:
private:
	int			_idx;
	Aggregate	*_ag;
};
 
class ConcreteAggregate : public Aggregate
{
public:
	ConcreteAggregate()
	{
		for (int i=0; i<SIZE; i++)
		{
			object[i] = i+1;
		}
	}
	virtual ~ConcreteAggregate()
	{
 
	}
	virtual Object getItem(int index)
	{
		return object[index];
	}
	virtual MyIterator *CreateIterator()
	{
		return new ContreteIterator(this);
	}
	virtual int getSize()
	{
		return SIZE;
	}
protected:
private:
	Object object[SIZE];
};
 
int main()
{
	// 创建一个集合
	Aggregate *ag = new ConcreteAggregate();
	// 创建一个遍历这个集合的 迭代器
	MyIterator *it = ag->CreateIterator();
	//通过迭代器 遍历 集合
	for (; !(it->IsDone()); it->Next() )
	{
		cout << it->CurrentItem() << " ";
	}
	
	//清理相关资源 
	delete it;
	delete ag;
	return 0;
}