// content : for_each 
//    - en.cppreference.com
//    - modoocode.com/260
//    - nested loop

#include <vector>
#include <algorithm>
#include <iostream>

struct Sum {
    void operator()(int n) { sum += n; }
    int sum{0};
};

int cpprefernce_example() {
    std::vector<int> nums{3, 4, 2, 8, 15, 267};
    auto print = [](const int& n) { std::cout << " " << n; };

    std::cout << "before :";
    std::for_each(nums.cbegin(), nums.cend(), print);
    std::cout << '\n';

    std::for_each(nums.begin(), nums.end(), [](int &n) { n++; });

    // Calls Sum::operator() for each number
    Sum s = std::for_each(nums.begin(), nums.end(), Sum());

    std::cout << "after :";
    std::for_each(nums.cbegin(), nums.cend(), print);
    std::cout << '\n';
    std::cout << "sum: " << s.sum << '\n';

    return 0;
}

void myfunction(int i) {
    std::cout << ' ' << i;
}

struct myclass {
    void operator()(int i) { std::cout << ' ' << i; }
} myobject;

int modoocode_example() {
    std::vector<int> myvector{10, 20, 30};

    // 일반적인 함수 포인터 전달
    std::cout << "myvector contains:";
    for_each(myvector.begin(), myvector.end(), myfunction);
    std::cout << '\n';

    // Functor를 전달
    std::cout << "myvector contains:";
    for_each(myvector.begin(), myvector.end(), myobject);
    std::cout << '\n';

    // 람다 함수를 전달
    std::cout << "myvector contains:";
    for_each(myvector.begin(), myvector.end(), [](const int& n) { std::cout << ' ' << n; });
    std::cout << '\n';

    return 0;
}

int my_example() {
    std::vector<int> first_vector{1, 2, 3, 4};
    std::vector<int> second_vector{2, 3, 4};

    for_each(first_vector.begin(), first_vector.end(), [&second_vector](int first) {
        for_each(second_vector.begin(), second_vector.end(), [&first](int second) {
            std::cout << first + second << " ";
        });
        std::cout << "\n";
    });

    return 0;
}

int main() {
    cpprefernce_example();
    modoocode_example();
    my_example();
    return 0;
}