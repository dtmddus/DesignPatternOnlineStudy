// Head First Design Pattern: Image Proxy Example
// modified from https://refactoring.guru/design-patterns/proxy/cpp/example

#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <future>
#include <mutex>
using namespace std::chrono_literals;

class Subject {
public:
    virtual void Request() = 0;
};

class RealSubject : public Subject {
public:
    RealSubject() {
        std::this_thread::sleep_for(2000ms);
    }
    void Request() override {
        std::cout << "RealSubject: handling request.\n";
    }
};

class ImageProxy : public Subject {
private:
    std::unique_ptr<RealSubject> real_subject_;
    bool started_ = false;
    bool loaded_ = false;
    std::mutex mtx;
    bool CheckAccess() const {
        std::cout << "Proxy: checking access prior to firing a real request.\n";
        return loaded_;
    }
    void LogAccess() const {
        std::cout << "Proxy: logging the time of request.\n";
    }
public:
    void Request() override {
        if (this->CheckAccess()) {
            this->real_subject_->Request();
        }
        else {  
            std::cout << "Proxy: loading Real Subject ..\n";       
           
            std::scoped_lock<std::mutex> lock{mtx};
            if (!started_) {
                started_ = true;
                real_subject_ = std::make_unique<RealSubject>();
                loaded_ = true;
                std::cout << "Proxy: completed to load datasets!\n";
            }
            this->real_subject_->Request();
        }
        this->LogAccess();
    }
};

void ClientCode(const std::shared_ptr<Subject> &subject) {
    subject->Request();
}

int main() {
    std::cout << "Client: Executing the same client code with a ImageProxy:\n";
    std::shared_ptr<ImageProxy> image_proxy = std::make_shared<ImageProxy>();
    auto first = std::async(std::launch::async, ClientCode, image_proxy);

    std::cout << std::endl;
    auto second = std::async(std::launch::async, ClientCode, image_proxy);

    std::this_thread::sleep_for(3000ms);
    std::cout << std::endl;
    auto third = std::async(std::launch::async, ClientCode, image_proxy);
    return 0;
}