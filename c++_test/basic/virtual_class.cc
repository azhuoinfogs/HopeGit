#include <iostream>
using namespace std;
class Animal {
public:
    virtual void makeSound() {
        cout << "The animal makes a sound.\n";
    }
    virtual double getArea() = 0;
    virtual ~Animal() {
        cout << "The animal is deleted \n";
    }
};

class Cat : public Animal {
public:
    void makeSound() {
        cout << "Meow!\n";
    }
    double getArea() {
        cout << "Meow! getArea\n";
        return 0;
    }
    ~Cat() {
        cout << "The Cat animal is deleted \n";
    }
};

class Dog : public Animal {
public:
    void makeSound() {
        cout << "Woof!\n";
    }
    double getArea() {
        cout << "Woof! getArea\n";
        return 0;
    }
    ~Dog() {
        cout << "The Dog animal is deleted \n";
    }
};
int main() {
    Cat cat;
    cat.makeSound();
    Dog dog;
    dog.makeSound();
}