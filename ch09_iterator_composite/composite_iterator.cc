#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <memory>

using namespace std;

class Employee;
class IIterator {
public:
    virtual bool hasNext() = 0;
    virtual shared_ptr<Employee> next() = 0;
};

class UnlimitedGroupIterator : public IIterator {
public:
    UnlimitedGroupIterator(vector<shared_ptr<Employee>>& members) : members_(members) {}
    bool hasNext() {
        if (position < members_.size()) {
            return true;
        }
        else {
            return false;
        }
    }
    shared_ptr<Employee> next() {
        return members_[position++];
    }
private:
    vector<shared_ptr<Employee>>& members_;
    int position = 0;
};

class LimitedGroupIterator : public IIterator  {
public:
    LimitedGroupIterator(array<shared_ptr<Employee>, 5>& members) : members_(members) {}
    bool hasNext() {
        if (position < max_member && members_[position]) {
            return true;
       }
        else {
            return false;
        }
    }
    shared_ptr<Employee> next() {
        return members_[position++];
    }
private:
    constexpr static int max_member = 5;
    array<shared_ptr<Employee>, max_member>& members_;
    int position = 0;
};

class IGroup {
public:
    virtual IIterator* createIterator() = 0;
    virtual void add(shared_ptr<Employee> e) = 0;
    virtual void remove(shared_ptr<Employee> e) = 0;
};

class UnlimitedGroup : public IGroup {
public:
    IIterator* createIterator() {
        return new UnlimitedGroupIterator(unlimited_members);
    }
    void add(shared_ptr<Employee> e) override {
        unlimited_members.push_back(e);
    }

    void remove(shared_ptr<Employee> e) override {
        unlimited_members.erase(find(unlimited_members.begin(), unlimited_members.end(), e));
    }
private:
    vector<shared_ptr<Employee>> unlimited_members; 
};

class LimitedGroup : public IGroup {
public:
    IIterator* createIterator() {
        return new LimitedGroupIterator(limited_members);
    }
    void add(shared_ptr<Employee> e) override {
        if (position < 5) {
            limited_members[position++] = e;
        }
    }
    void remove(shared_ptr<Employee> e) override {
        if (position == 0) {
            return;
        }
        int remove_position;
        for (int i = 0; i < limited_members.size(); i++) {
            if (limited_members[i] == e) {
                remove_position = i;
                break;
            }
        }
        for (int i = remove_position; i < limited_members.size()-1; i++) {
            limited_members[i] = limited_members[i+1];
        }
    }
private:
    array<shared_ptr<Employee>, 5> limited_members = {nullptr, };
    int position = 0;
};

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

class Developer final : public Employee {
public:
    Developer(string name, string position, int salary) : Employee(name, position, salary, "Developer") {}
    virtual ~Developer() {}
    void print() override {
        Employee::print();
    }
};

class Designer final : public Employee {
public:
    Designer(string name, string position, int salary) : Employee(name, position, salary, "Designer") {}
    virtual ~Designer() {}
    void print() override {
        Employee::print();
    }
};

template <class T>
class Manager : public Employee {
public:
    Manager(string name, string position, int salary) : Employee(name, position, salary, "Manager"), group_(make_unique<T>()) {}
    virtual ~Manager() {}
    void add(shared_ptr<Employee> e) override {
        group_->add(e);
    }
    void remove(shared_ptr<Employee> e) override {
        group_->remove(e);
    }
    void print() override {
        Employee::print();
        cout << endl;
        IIterator* iterator = group_->createIterator();
        while (iterator->hasNext()) {
            shared_ptr<Employee> e = iterator->next();
            e->print();
            cout << endl;
        }
        delete iterator;
    }
private:
    unique_ptr<T> group_;
};

using EmployeeSPtr = shared_ptr<Employee>;
struct EmployeeFactory {
    static EmployeeSPtr createUnlimitedManager(string name, string postion, int salary) {
        return make_shared<Manager<UnlimitedGroup>>(name, postion, salary);
    }
    static EmployeeSPtr createLimitedManager(string name, string postion, int salary) {
        return make_shared<Manager<LimitedGroup>>(name, postion, salary);
    }
    static EmployeeSPtr createDeveloper(string name, string postion, int salary) {
        return make_shared<Manager<UnlimitedGroup>>(name, postion, salary);
    }
    static EmployeeSPtr createDesigner(string name, string postion, int salary) {
        return make_shared<Manager<UnlimitedGroup>>(name, postion, salary);
    }
};

int main() {
    auto ceo = EmployeeFactory::createUnlimitedManager("Luka", "CEO", 100);
    auto messenger_lead = EmployeeFactory::createLimitedManager("Jake", "MassengerLead", 50);
    auto commerce_lead = EmployeeFactory::createUnlimitedManager("Benjamin", "CommerceLead", 80);
    ceo->add(messenger_lead);
    ceo->add(commerce_lead);

    auto messenger_developer = EmployeeFactory::createDeveloper("Kim", "MassengerDeveloper", 30);
    auto messenger_designer = EmployeeFactory::createDesigner("Alice", "MassengerDesigner", 20);
    messenger_lead->add(messenger_developer);
    messenger_lead->add(messenger_designer);

    auto commerce_developer = EmployeeFactory::createDeveloper("Jason", "CommerceDeveloper", 40);
    auto commerce_designer = EmployeeFactory::createDesigner("Sunny", "CommerceDesigner", 25);
    auto commerce_second_designer = EmployeeFactory::createDesigner("Helen", "CommerceDesigner", 25);
    commerce_lead->add(commerce_developer);
    commerce_lead->add(commerce_designer);
    commerce_lead->add(commerce_second_designer);
    
    ceo->print();
    commerce_lead->remove(commerce_designer);

    ceo->print();

    return 0;
}