#include <iostream>
#include <string>
#include <memory>
#include <vector>
using namespace std;

class Operator {
public:
    Operator() {};
    virtual ~Operator() {};
    virtual string description() = 0;
    virtual string print() = 0;
    virtual int calculate(int a, int b) = 0;
};

class Add : public Operator { 
public:
    Add() {}
    ~Add() {}
    string description() {
        return "Add Operator";
    }
    string print() {
        return "+";
    }
    int calculate(int a, int b) {
        return a + b;
    }  
};

class Sub : public Operator { 
public:
    Sub() {}
    ~Sub() {}
    string description() {
        return "Sub Operator";
    }
    string print() {
        return "-";
    }
    int calculate(int a, int b) {
        return a - b;
    }  
};

class Mul : public Operator { 
public:
    Mul() {}
    ~Mul() {}
    string description() {
        return "Mul Operator";
    }
    string print() {
        return "*";
    }
    int calculate(int a, int b) {
        return a * b;
    }  
};

class Div : public Operator { 
public:
    Div() {}
    ~Div() {}
    string description() {
        return "Div Operator";
    }
    string print() {
        return "/";
    }
    int calculate(int a, int b) {
        return a / b;
    }  
};

class Calculator {
public:
    Calculator() {}
    virtual ~Calculator() {}
    void calculate() {
        res = op_->calculate(a_, b_);
        print();
    }

    void setOperator(shared_ptr<Operator> op) {
        op_ = op;
    }

private:
    // 계산 안된 상태에서 호출 방지
    void print() {
        cout << op_->description() << ": " << a_ << op_->print() << b_ << " = " << res << endl;
    }

protected:
    shared_ptr<Operator> op_ = nullptr;
    int res = 0;
    int a_ = 0;
    int b_ = 0;
};

class AddCalculator : public Calculator {
public:
    AddCalculator(int a, int b) {
        a_ = a;
        b_ = b;
        op_ = make_unique<Add>();
    }
    ~AddCalculator() = default;
};

class SubCalculator : public Calculator {
public:
    SubCalculator(int a, int b) {
        a_ = a;
        b_ = b;
        op_ = make_unique<Sub>();
    }
    ~SubCalculator() = default;
};

class MulCalculator : public Calculator {
public:
    MulCalculator(int a, int b) {
        a_ = a;
        b_ = b;
        op_ = make_unique<Mul>();
    }
    ~MulCalculator() = default;
};

class DivCalculator : public Calculator {
public:
    DivCalculator(int a, int b) {
        a_ = a;
        b_ = b;
        op_ = make_unique<Div>();
    }
    ~DivCalculator() = default;
};


int main() {
    int a = 4, b = 2;

    shared_ptr<Calculator> cal_add = make_shared<AddCalculator>(a, b);
    cal_add->calculate();
    shared_ptr<Calculator> cal_sub = make_shared<SubCalculator>(a, b);
    cal_sub->calculate();
    shared_ptr<Calculator> cal_mul = make_shared<MulCalculator>(a, b);
    cal_mul->calculate();
    shared_ptr<Calculator> cal_div = make_shared<DivCalculator>(a, b);
    cal_div->calculate();

    cout << "You can swap operator (algorithm) at runtime!" << endl;
    shared_ptr<Calculator> calculator = make_shared<DivCalculator>(a, b);
    shared_ptr<Operator> add_operator = make_shared<Add>();
    calculator->setOperator(add_operator);
    calculator->calculate();
    shared_ptr<Operator> sub_operator = make_shared<Sub>();
    calculator->setOperator(add_operator);
    calculator->calculate();
    
    return 0;
}