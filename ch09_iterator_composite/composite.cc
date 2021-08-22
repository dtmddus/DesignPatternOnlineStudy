#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// base component class
class Employee {
public:
    Employee(string name, string position, int salary, string duty) : name_(name), position_(position), salary_(salary), duty_(duty) {}
    virtual ~Employee() {}
    virtual void add(shared_ptr<Employee> e) {}
    virtual void remove(shared_ptr<Employee> e) {}
    virtual void print() {
        cout << "Name : " << getName() << " (" << getDuty() << ")";
        cout << "\nPosition : " << getPosition();
        cout << "\nSalary : " << getSalary() << endl;
    }
public: 
    virtual string getName() final { return name_; }
    virtual string getDuty() final { return duty_; }
    virtual string getPosition() final { return position_; }
    virtual int getSalary() final { return salary_; }
private:
    string name_;
    string duty_;
    string position_;
    int salary_;
};

// leaf class
class Developer final : public Employee {
public:
    Developer(string name, string position, int salary) : Employee(name, position, salary, "Developer") {}
    virtual ~Developer() {}
    void print() override {
        Employee::print();
    }
};

// leaf class
class Designer final : public Employee {
public:
    Designer(string name, string position, int salary) : Employee(name, position, salary, "Designer") {}
    virtual ~Designer() {}
    void print() override {
        Employee::print();
    }
};

// composite class
class Manager final : public Employee {
public:
    Manager(string name, string position, int salary) : Employee(name, position, salary, "Manager") {}
    virtual ~Manager() {}
    void add(shared_ptr<Employee> e) override {
        members_.push_back(e);
    }
    void remove(shared_ptr<Employee> e) override {
        members_.erase(find(members_.begin(), members_.end(), e));
    }
    void print() override {
        Employee::print();
        cout << endl;
        for (auto member: members_) {
            member->print();
            cout << endl;
        }
    }
private:
    vector<shared_ptr<Employee>> members_;
};


int main() {
    shared_ptr<Employee> ceo = make_shared<Manager>("Luka", "CEO", 100);
    shared_ptr<Employee> messenger_lead = make_shared<Manager>("Jake", "MassengerLead", 50);
    shared_ptr<Employee> commerce_lead = make_shared<Manager>("Benjamin", "CommerceLead", 80);
    ceo->add(messenger_lead);
    ceo->add(commerce_lead);

    shared_ptr<Employee> messenger_developer = make_shared<Developer>("Kim", "MassengerDeveloper", 30);
    shared_ptr<Employee> messenger_designer = make_shared<Designer>("Alice", "MassengerDesigner", 20);
    messenger_lead->add(messenger_developer);
    messenger_lead->add(messenger_designer);

    shared_ptr<Employee> commerce_developer = make_shared<Developer>("Jason", "CommerceDeveloper", 40);
    shared_ptr<Employee> commerce_designer = make_shared<Designer>("Sunny", "CommerceDesigner", 25);
    shared_ptr<Employee> commerce_second_designer = make_shared<Designer>("Helen", "CommerceDesigner", 25);
    commerce_lead->add(commerce_developer);
    commerce_lead->add(commerce_designer);
    commerce_lead->add(commerce_second_designer);
    
    ceo->print();
    commerce_lead->remove(commerce_designer);

    ceo->print();
    return 0;
}