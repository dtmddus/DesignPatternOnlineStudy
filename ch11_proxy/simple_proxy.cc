// Proxy Pattern Example
// reference: https://refactoring.guru/design-patterns/proxy/cpp/example

#include <iostream>
#include <memory>

class Subject {
public:
    virtual void Request() const = 0;
};

class RealSubject : public Subject {
public:
    void Request() const override {
        std::cout << "RealSubject: handling request.\n";
    }
};

class Proxy : public Subject {
private:
    std::shared_ptr<RealSubject>& real_subject_;
    bool CheckAccess() const {
        std::cout << "Proxy: checking access prior to firing a real request.\n";
        return true;
    }
    void LogAccess() const {
        std::cout << "Proxy: Logging the time of request.\n";
    }
public:
    Proxy(std::shared_ptr<RealSubject>& real_subject) : real_subject_(real_subject) {}
    ~Proxy() {}
    void Request() const override {
        if (this->CheckAccess()) {
            this->real_subject_->Request();
            this->LogAccess();
        }
    }
};

void ClientCode(const std::shared_ptr<Subject> &subject) {
    subject->Request();
}

int main() {
    std::cout << "Client: Executing the client code with a real subject:\n";
    std::shared_ptr<RealSubject> real_subject = std::make_shared<RealSubject>();
    ClientCode(real_subject);
    std::cout << std::endl;

    std::cout << "Client: Executing the same client code with a proxy:\n";
    std::shared_ptr<Proxy> proxy = std::make_shared<Proxy>(real_subject);
    ClientCode(proxy);

    return 0;
}

