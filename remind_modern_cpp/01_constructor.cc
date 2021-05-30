#include <iostream>
#include <string>
using namespace std;

class Cat {
  public:
    // Cat() = default;
    Cat() { 
        cout << "constructor 1" << endl;
    }

    Cat(string name, int age)
    : name_(move(name)), age_(age) {
        cout << "constructor 2" << endl;
    }

    template<typename T> 
    Cat(string name, T age) = delete;
    // Cat(string name, float age) = delete;

    // ~Cat() noexcept {}
    ~Cat() = default;

    Cat(const Cat& other) 
    : name_(other.name_), age_(other.age_) {
        cout << "copy constructor" << endl;
    }

    Cat(Cat&& other) noexcept
    : name_(move(other.name_)), age_(other.age_) {
        cout << "move constructor" << endl;
    }

    Cat& operator=(const Cat& other) {
        if (&other == this) {
            return *this;
        }
        name_ = other.name_;
        age_ = other.age_;
        cout << "copy assignment" << endl;
        return *this;
    }

    Cat& operator=(Cat&& other) noexcept {
        if (&other == this) {
            return *this;
        }
        name_ = move(other.name_);
        age_ = other.age_;
        cout << "move assignment" << endl;
        return *this;
    }

  private:
    string name_;
    int age_;
    
    /*
        float* ptr_;

        위와 같이 포인터를 사용하여 멤버변수를 사용하는 경우에
        deep copy를 위해서 copy/move constructor, copy/move assignment를 정의해서
        데이터를 복사 또는 이동시켜야 한다.

        그렇지 않는 경우에는,
        컴파일러가 4개의 copy/move 함수를 default로 자동 생성하므로 직접 정의하지 않아도 된다!
    */
};

int main() {
    Cat cat_a("white", 10);
    // Cat cat_err("white", 10.1f);  // compile time error: use of deleted function

    Cat cat_b(cat_a);        // copy constructor
    Cat cat_c = cat_a;       // copy constructor
    Cat cat_d(move(cat_c));  // move constructor

    cat_c = cat_b;           // copy assignment
    cat_c = move(cat_b);     // move assignment

    return 0;
}