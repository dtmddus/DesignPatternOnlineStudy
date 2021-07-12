#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <random>
#include <chrono>
#include <set>

using namespace std;

template<typename ... Args> 
std::string string_format(const std::string& format, Args ... args) {
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    if (size <= 0) {
        throw std::runtime_error("Error during formatting.");
    }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1);
}

// Receiver
class ILight {
public: 
    virtual void on() const = 0;
    virtual void off() const = 0;
    int getDelay() { return 1; };
    virtual string getName() const = 0;
};

class LivingRoomLight : public ILight {
public:
    void on() const override { cout << "LivingRoomLight on .. "; }
    void off() const override { cout << "LivingRoomLight off .. "; }
    string getName() const override { return "LivingRoomLight"; }
};

class MainRoomLight : public ILight {
public:
    void on() const override { cout << "MainRoomLight on .. "; }
    void off() const override { cout << "MainRoomLight off .. "; }
    string getName() const override { return "MainRoomLight"; }
};

class IElectronics {
public:
    virtual void start() const = 0;
    virtual void stop() const = 0;
    virtual void resume() const = 0;
    virtual int getDelay() const = 0;
    virtual string getName() const = 0;
};

class WashingMachine : public IElectronics {
public:
    void start() const override { cout << "WashingMachine started .. "; }
    void stop() const override { cout << "WashingMachine stopped .. "; }
    void resume() const override { cout << "WashingMachine resumed .. "; }
    int getDelay() const override { return 60; };
    string getName() const override { return "WashingMachine"; }
};

class Microwave : public IElectronics {
public:
    void start() const override { cout << "Microwave started .. "; }
    void stop() const override { cout << "Microwave stopped .. "; }
    void resume() const override { cout << "Microwave resumed .. "; }
    int getDelay() const override { return 10; };
    string getName() const override { return "Microwave"; }
};

class IChore {
public:
    virtual void doSomething() const = 0;
    virtual int getDelay() const = 0;
    virtual string getName() const = 0;
};

class Flowerpot : public IChore {
public:
    void doSomething () const override {
        water();
        prune();
    }
    int getDelay() const override { return 30; };
    string getName() const override { return "Flowerpot"; }
private:
    void water() const { cout << "Flowerpot watered .. "; }
    void prune() const { cout << "Flowerpot pruned .. "; }
};

class Postbox : public IChore {
public:
    void doSomething () const override {
        confirm();
    }
    int getDelay() const override { return 5; };
    string getName() const override { return "Postbox"; }
private:
    void confirm() const { cout << "Postbox confirmed .. "; }
};

// Commnad
class IJob {
public:
    virtual ~IJob() = default;
    virtual void execute() const = 0;
    void setPriority(const int priority) { priority_ = priority; }
    int getPriority() const { return priority_; }
    void setDuration(const int duration) { duration_ = duration; }
    int getDuration() const { return duration_; }
    virtual string getName() const = 0;
protected:
    void printPriorityDelay() const {
        string message = string_format("(priority : %d, delay : %d)", priority_, duration_);
        cout << message << endl;
    }
private:
    int priority_ = 0;
    int duration_ = 0;
};

// Invoker
class compare_level {
public:
    bool operator() (IJob* lhs, IJob* rhs) {
        if (lhs->getPriority() == rhs->getPriority()) {
            return lhs->getDuration() > rhs->getDuration();
        }
        return lhs->getPriority() > rhs->getPriority();
    }
};

class HouseKeeper {
public:
    HouseKeeper() = default;
    ~HouseKeeper() = default;
    void pushJob(IJob* job) {
        cout << job->getName() << endl;
        queue_.push(job);
        last_queue_.push(job);
        while (last_queue_.size() > 3) {
            last_queue_.pop();
        }
    } 
    void execute() {
        cout << "[2] HouseKeeper started" << endl;
        while (!queue_.empty()) {
            queue_.top()->execute();
            queue_.pop();
        }
    }
    void lastJob() {
        cout << "[3] HouseKeeper redo" << endl;
        while(!last_queue_.empty()) {
            last_queue_.front()->execute();
            last_queue_.pop();
        }
    }
private:
    priority_queue<IJob*, vector<IJob*>, compare_level> queue_;
    queue<IJob*> last_queue_;
};

// Concrete Command
class LightOnJob : public IJob {
public:
    LightOnJob(ILight &light) : light_(light) {
        setPriority(1);
        setDuration(light.getDelay());
    }
    void execute() const override {
        light_.on();
        printPriorityDelay();
    }
    string getName() const override { return light_.getName(); }
private:
    ILight &light_;
};

class HomeApplianceOnJob : public IJob {
public:
    HomeApplianceOnJob(IElectronics &electronics) : electronics_(electronics) {
        setPriority(2);
        setDuration(electronics.getDelay());
    }
    void execute() const override {
        electronics_.start();
        printPriorityDelay();
    }
    string getName() const override { return electronics_.getName(); }
private:
    IElectronics &electronics_;
};

class HouseChoreJob : public IJob {
public:
    HouseChoreJob(IChore &chore) : chore_(chore) {
        setPriority(3);
        setDuration(chore.getDelay());
    }
    void execute() const override {
        chore_.doSomething();
        printPriorityDelay();
    }
    string getName() const override { return chore_.getName(); }
private:
    IChore &chore_;
};

void test() {
    unique_ptr<HouseKeeper> iam = make_unique<HouseKeeper>();  

    constexpr unsigned int job_count = 6;
    vector<shared_ptr<IJob>> job_list(job_count);

    // chores
    Flowerpot flowerpot;
    Postbox postbox;
    job_list[0] = make_shared<HouseChoreJob>(flowerpot);
    job_list[1] = make_shared<HouseChoreJob>(postbox);

    // light on
    LivingRoomLight living_room_light;
    MainRoomLight main_room_light;
    job_list[2] = make_shared<LightOnJob>(living_room_light);
    job_list[3]= make_shared<LightOnJob>(main_room_light);

    // electronics start
    WashingMachine washing_machine;
    Microwave microwave;
    job_list[4] = make_shared<HomeApplianceOnJob>(washing_machine);
    job_list[5] = make_shared<HomeApplianceOnJob>(microwave);

    auto current = std::chrono::system_clock::now();
    auto duration = current.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    mt19937_64 gen(millis);
    uniform_int_distribution<__int64> distrib(0, 5);
    set<int> job_idxs;
    cout << "[1] Push Job" << endl;
    for (int i = 0; i < job_count; i++) {
        int rnd = distrib(gen);
        while (job_idxs.find(rnd) != job_idxs.end()) {
            rnd = distrib(gen);
        }
        job_idxs.insert(rnd);
        iam->pushJob(job_list[rnd].get());
    }

    iam->execute();
    iam->lastJob();
}

int main() {
    for (int i = 0; i < 3; i++) {
        test();
        cout << endl;
    }
    return 0;
}
