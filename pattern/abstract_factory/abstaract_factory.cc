#pragma once
#include <iostream>

// 键盘
class KeyBoard
{
public:
	virtual void show() = 0;
};
// 微软的键盘
class KeyBoardMicro : public KeyBoard
{
public:
	void show()
	{
		std::cout << "微软的键盘" << std::endl;
	}
};
// 联想的键盘
class KeyBoardLenovo : public KeyBoard
{
public:
	void show()
	{
		std::cout << "联想的键盘" << std::endl;
	}
};
// 鼠标
class Mouse
{
public:
	virtual void show() = 0;
};

class MouseMicro : public Mouse
{
public:
	void show()
	{
		std::cout << "微软的鼠标" << std::endl;
	}
};

class MouseLenovo : public Mouse
{
public:
	void show()
	{
		std::cout << "联想的鼠标" << std::endl;
	}
};
// 工厂
class Factory
{
public:
	virtual KeyBoard * createKeyBoard() = 0;
	virtual Mouse * createMouse() = 0;
};
// 微软的工厂
class FactoryMicro : public Factory
{
public:
	KeyBoard * createKeyBoard()
	{
		return new KeyBoardMicro();
	}
	Mouse * createMouse()
	{
		return new MouseMicro();
	}
};
// 联想的工厂
class FactoryLenovo : public Factory
{
public:
	KeyBoard * createKeyBoard()
	{
		return new KeyBoardLenovo();
	}
	Mouse * createMouse()
	{
		return new MouseLenovo();
	}
};

int main()
{
	using namespace std;
	// 抽象工厂模式
	Factory * p = new FactoryMicro();
	KeyBoard * pKeyBoard = p->createKeyBoard();
	Mouse * pMouse = p->createMouse();
	pKeyBoard->show();
	pMouse->show();
	delete pMouse;
	delete pKeyBoard;
	delete p;

	p = new FactoryLenovo();
	pKeyBoard = p->createKeyBoard();
	pMouse = p->createMouse();
	pKeyBoard->show();
	pMouse->show();
	delete pMouse;
	delete pKeyBoard;
	delete p;
	getchar();
	return 0;
}