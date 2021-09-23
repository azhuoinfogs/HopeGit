#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
class ComponentPtr
{
protected:
	std::string m_strName;
public:
	ComponentPtr(std::string str)
	{
		m_strName = str;
	}
	virtual void add(ComponentPtr * p) = 0;
	virtual void remove(ComponentPtr * p) = 0;
	virtual void display() = 0;
};

class LeafPtr : public ComponentPtr
{
public:
	LeafPtr(std::string str) : ComponentPtr(str) {}
	void add(ComponentPtr * p)
	{
		std::cout << "Leaf cannot add" << std::endl;
	}
	void remove(ComponentPtr * p)
	{
		std::cout << "Leaf cannot remove" << std::endl;
	}
	void display()
	{
		std::cout << m_strName << std::endl;
	}
};

class CompositePtr : public ComponentPtr
{
private:
	// 这里使用智能指针不用自己释放new的内存
	std::vector<std::shared_ptr<ComponentPtr>> m_vec;
public:
	CompositePtr(std::string str) : ComponentPtr(str) {};
	~CompositePtr()
	{
		if (!m_vec.empty())
		{
			m_vec.clear();
		}
	}
	void add(ComponentPtr * p)
	{
		auto it = find_if(m_vec.begin(), m_vec.end(), 
			[p](std::shared_ptr<ComponentPtr> ptr) {return p == ptr.get(); });
		if (it == m_vec.end())
			m_vec.push_back(std::shared_ptr<ComponentPtr>(p));
	}
	void remove(ComponentPtr * p)
	{
		auto it = find_if(m_vec.begin(), m_vec.end(),
			[p](std::shared_ptr<ComponentPtr> ptr) {return p == ptr.get(); });
		if (it == m_vec.end())
			return;
		m_vec.erase(it);
	}
	void display()
	{
		for (auto it = m_vec.cbegin(); it != m_vec.cend(); it++)
		{
			(*it)->display();
		}
	}
};
using namespace std;
int main()
{
	// 组合模式
	CompositePtr * p = new CompositePtr("总部");
	p->add(new LeafPtr("总部财务部门"));
	p->add(new LeafPtr("总部人力资源部门"));
	CompositePtr * p1 = new CompositePtr("上海分部");
	p1->add(new LeafPtr("上海分部财务部门"));
	p1->add(new LeafPtr("上海分部人力资源部门"));
	p->add(p1);

	p->display();
	return 0;
}