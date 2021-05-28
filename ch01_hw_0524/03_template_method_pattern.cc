#include <iostream>
#include <vector>
#include <memory>

class Operator {
public:
    Operator() {}
    Operator(int a, int b) : a_(a), b_(b) {}
    ~Operator() {}
    virtual int calculate() { 
        return -1;
    };

protected:
    int a_;
    int b_;
};

class Add : public Operator {
public:
    Add(int a, int b) {
        a_ = a; 
        b_ = b;
    }
    ~Add() {}
    int calculate() {
        return a_ + b_;
    }
};

class Sub : public Operator {
public:
    Sub(int a, int b) {
        a_ = a; 
        b_ = b;
    }
    ~Sub() {}
    int calculate() {
        return a_ - b_;
    }
};

class Div : public Operator {
public:
    Div(int a, int b) {
        a_ = a; 
        b_ = b;
    }
    ~Div() {}
    int calculate() {
        return a_ / b_;
    }
};

int main() {
    int a = 5, b = 3;
    
    std::shared_ptr<Operator> op = std::make_shared<Operator>(a, b);
    std::cout << op->calculate() << std::endl;
    
    std::shared_ptr<Operator> add = std::make_shared<Add>(a, b);
    std::cout << add->calculate() << std::endl;

    std::shared_ptr<Operator> sub = std::make_shared<Sub>(a, b);
    std::cout << sub->calculate() << std::endl;

    std::shared_ptr<Operator> div = std::make_shared<Div>(a, b);
    std::cout << div->calculate() << std::endl;

    return 0;
}