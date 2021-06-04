// reference: https://micuninja.com/observer-pattern-shared_ptr-perspective/

#include <iostream>
#include <memory>
#include <set>
#include <list>
#include <vector>
using namespace std;

class IObserver : public enable_shared_from_this<IObserver> {
public:
    virtual ~IObserver() = default;
    virtual void update(int cpu) = 0;
};

class IResourceProvider {
public:
    virtual ~IResourceProvider() = default;
    virtual void attach(const shared_ptr<IObserver>& ob) = 0;
    virtual void detach(const shared_ptr<IObserver>& ob) = 0;
    virtual void setMeasurement(int cpu) = 0;
protected:
    virtual void notify() = 0;
};

class ResourceProvider : public IResourceProvider {
public:
    void attach(const shared_ptr<IObserver>& ob) {
        machine_.push_back(ob);
        cout << machine_.size() << endl;
    }
    void detach(const shared_ptr<IObserver>& ob) {
        machine_.remove(ob);
    }
    void setMeasurement(int cpu) {
        cpu_ = cpu;
        notify();
    }
protected:
    void notify() override {
        for (shared_ptr<IObserver> it: machine_) {
            it->update(cpu_);
        }
    }
private:
    list<shared_ptr<IObserver>> machine_;
    int cpu_;
};

unsigned int machine_id_count = 0;
class DisplayMachine : public IObserver {
public: 
    DisplayMachine() {
        machine_id_ = machine_id_count++;
        //provider_.attach(shared_ptr<IObserver>(this));
    }
    void attach(const shared_ptr<ResourceProvider>& provider) {
        provider_ = provider;
        provider_->attach(shared_from_this());
    }
    virtual ~DisplayMachine() {
        provider_->detach(shared_from_this());
    }
    void update(int cpu) override {
        cpu_ = cpu;
        display();
    }
    void display() {
        cout << "Machine " << machine_id_ << " : " << cpu_ << endl;
    }
private:
    unsigned int machine_id_;
    int cpu_;
    shared_ptr<ResourceProvider> provider_;
};

int main() {
    shared_ptr<ResourceProvider> provider = make_shared<ResourceProvider>();
    DisplayMachine display_machine_red;
    display_machine_red.attach(provider);
    DisplayMachine display_machine_blud;
    display_machine_blud.attach(provider);
    DisplayMachine display_machine_green;
    display_machine_green.attach(provider);

    const int msg_count = 5;
    for (int i = 0; i < msg_count; i++) {
        provider->setMeasurement(i*10);
    }
    return 0;
}