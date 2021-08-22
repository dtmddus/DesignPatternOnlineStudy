#include <iostream>
#include <memory>
#include <set>
using namespace std;

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void update(int cpu) = 0;
    virtual unsigned int get_id() = 0;
};

class IResourceProvider {
public:
    virtual ~IResourceProvider() = default;
    virtual void attach(IObserver* ob) = 0;
    virtual void detach(IObserver* ob) = 0;
    virtual void setMeasurement(int cpu) = 0;
protected:
    virtual void notify() = 0;
};

struct compare_machine {
    bool operator() (IObserver* const lhs, IObserver* const rhs) {
        return lhs->get_id() < rhs->get_id();
    }
};

class ResourceProvider : public IResourceProvider {
public:
    void attach(IObserver* ob) override {
        observers_.insert(ob);
        cout << "Number of Observers : " << observers_.size() << endl;
    }
    void detach(IObserver* ob) override {
        observers_.erase(ob);
    }
    void setMeasurement(int cpu) {
        cpu_ = cpu;
        notify();
    }
protected:
    void notify() override {
        for (auto& ob: observers_) {
            ob->update(cpu_);
        }
    }
private:
    set<IObserver*, compare_machine> observers_;
    int cpu_;
};

unsigned int machine_id_count = 0;
class DisplayMachine : public IObserver {
public: 
    DisplayMachine(ResourceProvider& provider) : provider_(provider)  {
        provider_.attach(this);
        machine_id_ = machine_id_count++;
    }
    virtual ~DisplayMachine() {
        provider_.detach(this);
    }
    void update(int cpu) override {
        cpu_ = cpu;
        display();
    }
    void display() {
        cout << "Machine " << machine_id_ << " : " << cpu_ << endl;
    }
    unsigned int get_id() {
        return machine_id_;
    }
private:
    unsigned int machine_id_;
    int cpu_;
    ResourceProvider& provider_;
};

int main() {
    ResourceProvider provider;
    DisplayMachine display_machine_red(provider);
    DisplayMachine display_machine_blue(provider);
    DisplayMachine display_machine_green(provider);

    const int msg_count = 5;
    for (int i = 0; i < msg_count; i++) {
        provider.setMeasurement(i*10);
    }
    return 0;
}