#include <iostream>
#include <string>
using namespace std;

class Context {   // 包含解释器之外的全局信息
private:
    string input;
    string output;
public:
    void SetInput(string i) { input = i; }
    string GetInput() { return input; }
    void SetOutput(string o) { output = o; }
    string GetOutput() { return output; }
};

class AbstractExpression {
public:
    virtual void Interpret(Context* context) = 0;
    virtual ~AbstractExpression() {}
};

class TerminalExpression : public AbstractExpression {  // 终结符表达式
public:
    void Interpret(Context* context) {
        cout << "TerminalExpression: " << context->GetInput() << ", " << context->GetOutput() << endl;
    }
};

class NonterminalExpression : public AbstractExpression {   // 非终结符表达式
private:
    AbstractExpression* expression;
public:
    NonterminalExpression(AbstractExpression* e) { expression = e; }
    void Interpret(Context* context) {
        cout << "NonterminalExpression: " << context->GetInput() << ", " << context->GetOutput() << endl;
        expression->Interpret(context);
    }
};

int main() {
    Context* c = new Context();
    c->SetInput("Hello");
    c->SetOutput("World");

    AbstractExpression* exp1 = new TerminalExpression();
    AbstractExpression* exp2 = new NonterminalExpression(exp1);
    exp1->Interpret(c); // TerminalExpression: Hello, World
    exp2->Interpret(c); // NonterminalExpression: Hello, World
                        // TerminalExpression: Hello, World
    delete exp1;
    delete exp2;
}