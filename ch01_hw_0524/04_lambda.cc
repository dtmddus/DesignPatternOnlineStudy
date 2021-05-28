#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>

class Div {
public:
    int operator()(const int a, const int b) {
        return a / b;
    } 
};

int main2() {
    std::unordered_map<std::string, std::function<int(const int, const int)>> operator_map;
    auto add_f = [](const int a, const int b) -> int { return a+b; };
    auto sub_f = [](const int a, const int b) -> int { return a-b; };
    operator_map["Add"] = add_f;
    operator_map["Sub"] = sub_f;
    // operator_map["Div"] = div();
    operator_map["Div"] = [](const int a, const int b) -> int { return a/b; };
    operator_map["Mul"] = [](const int a, const int b) -> int { return a*b; };

    int a = 4, b = 2;
    std::vector<std::string> operators = {"Add", "Sub", "Div", "Mul"};
    for (auto& s: operators) {
        std::cout << a << s << b << " = " << operator_map[s](a, b) << std::endl;
    }
    return 0;
}