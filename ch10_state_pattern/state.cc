#include <iostream>
#include <string>
#include <memory>

using namespace std;

class State {
public:
    virtual void insertQuarter() = 0;
    virtual void ejectQuarter() = 0;
    virtual void turnCrank() = 0;
    virtual void dispense() = 0;
};

class GumballMachine {
public:
    GumballMachine(const int number_gumballs = 10);
    void insertQuarter() {
        current_state_->insertQuarter();
    }
    void ejectQuarter() {
        current_state_->ejectQuarter();
    }
    void turnCrank() {
        current_state_->turnCrank();
        current_state_->dispense();
    }
    void setState(shared_ptr<State>& state) {
        current_state_ = state;
    }
    void releaseBall() {
        cout << "알맹이가 나가고 있습니다." << endl;
        if (count != 0) {
            count = count - 1;
        }
    }
    int getCount() {
        return count;
    }
    shared_ptr<State>& getSoldOut() {
        return sold_out_;
    }
    shared_ptr<State>& getNoQuarter() {
        return no_quarter_;
    }
    shared_ptr<State>& getHasQuarter() {
        return has_quarter_;
    }
    shared_ptr<State>& getSold() {
        return sold_;
    }
    string operator()() {
        string str;
        str += "\n주식회사 왕뽑기\n";
        str += "남은 알맹이 개수 : " + to_string(count) + "\n";
        str += "동전 투입 대기중\n";
        return str;
    }

private:
    shared_ptr<State> sold_out_;
    shared_ptr<State> no_quarter_;
    shared_ptr<State> has_quarter_;
    shared_ptr<State> sold_;
    shared_ptr<State> current_state_;
    int count = 0;
};

class Sold : public State {
public:
    Sold(GumballMachine& gumball_machine) : gumball_machine_(gumball_machine) {}
    void insertQuarter() override {
        cout << "잠깐만 기다려 주세요. 알맹이가 나가고 있습니다." << endl;
    }
    void ejectQuarter() override {
        cout << "이미 알맹이를 뽑으셨습니다." << endl;
    }
    void turnCrank() override {
        cout << "손잡이를 한 번만 돌려주세요." << endl;
    }
    void dispense() override {
        gumball_machine_.releaseBall();
        if (gumball_machine_.getCount() > 0) {
            gumball_machine_.setState(gumball_machine_.getNoQuarter());
        }
        else {
            cout << "매진입니다." << endl;
            gumball_machine_.setState(gumball_machine_.getSoldOut());
        }
    }
private:
    GumballMachine& gumball_machine_;
};

class SoldOut : public State {
public:
    SoldOut(GumballMachine& gumball_machine) : gumball_machine_(gumball_machine) {}
    void insertQuarter() override {
        cout << "매진되었습니다. 다음 기회를 이용해주세요." << endl;
    }
    void ejectQuarter() override {
        cout << "매진되었습니다. 다음 기회를 이용해주세요." << endl;
    }
    void turnCrank() override {
        cout << "매진되었습니다. 다음 기회를 이용해주세요." << endl;
    }
    void dispense() override {
        cout << "매진되었습니다. 다음 기회를 이용해주세요." << endl;
    }
private:
    GumballMachine& gumball_machine_;
};

class NoQuarter : public State {
public:
    NoQuarter(GumballMachine& gumball_machine) : gumball_machine_(gumball_machine) {}
    void insertQuarter() override {
        cout << "동전을 넣었습니다." << endl;
        gumball_machine_.setState(gumball_machine_.getHasQuarter());
    }
    void ejectQuarter() override {
        cout << "동전을 넣어주세요" << endl;
    }
    void turnCrank() override {
        cout << "동전을 넣어주세요" << endl;
    }
    void dispense() override {
        cout << "동전을 넣어주세요" << endl;
    }
private:
    GumballMachine& gumball_machine_;
};

class HasQuarter : public State {
public:
    HasQuarter(GumballMachine& gumball_machine) : gumball_machine_(gumball_machine) {}
    void insertQuarter() override {
        cout << "동전을 한 개만 넣어주세요." << endl;
    }
    void ejectQuarter() override {
        cout << "동전이 반환 됩니다." << endl;
        gumball_machine_.setState(gumball_machine_.getNoQuarter());
    }
    void turnCrank() override {
        cout << "손잡이를 돌리셨습니다." << endl;
        gumball_machine_.setState(gumball_machine_.getSold());
    }
    void dispense() override {
        cout << "알맹이가 나갈 수 없습니다." << endl;
    }
private:
    GumballMachine& gumball_machine_;
};


GumballMachine::GumballMachine(const int number_gumballs) {
    sold_out_ = make_shared<SoldOut>(*this);
    no_quarter_ = make_shared<NoQuarter>(*this);
    has_quarter_ = make_shared<HasQuarter>(*this);
    sold_ = make_shared<Sold>(*this);
    current_state_ = sold_out_;
    count = number_gumballs;
    if (number_gumballs > 0) { 
        current_state_ = no_quarter_;
    }
}

int main() {
    GumballMachine gumball_machine(5);
    cout << gumball_machine() << endl;
    
    gumball_machine.insertQuarter();
    gumball_machine.turnCrank();

    cout << gumball_machine() << endl;

    gumball_machine.insertQuarter();
    gumball_machine.ejectQuarter();
    gumball_machine.turnCrank();

    cout << gumball_machine() << endl;    

    gumball_machine.insertQuarter();
    gumball_machine.turnCrank();
    gumball_machine.insertQuarter();
    gumball_machine.turnCrank();
    gumball_machine.ejectQuarter();

    cout << gumball_machine() << endl;    

    gumball_machine.insertQuarter();
    gumball_machine.insertQuarter();
    gumball_machine.turnCrank();
    gumball_machine.insertQuarter();
    gumball_machine.turnCrank();
    gumball_machine.insertQuarter();
    gumball_machine.turnCrank(); 
    
    cout << gumball_machine() << endl;
    return 0;
}
