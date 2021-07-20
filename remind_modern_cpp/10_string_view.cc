// string_view since c++17
// reference: learncpp.com

#include <iostream>
#include <string>
#include <string_view>
#include <cstring>

void test_intro_string_view() {
    {
        char text[] {"hello"};  // 1 copy (string literal -> char []) string literal (stored in binary at compile-time)
        std::string str{text};  // 1 copy
        std::string more{str};  // 1 copy
    }

    {
        // not any copies
        std::string_view text{"hello"};  // string_view has starting pointer and length from string literal
        std::string_view str{text};
        std::string_view more{str};
    }
}

// std::string_view is not only fast, but has many of functions that we know from std::string.
void test_string_functions() {
    std::string_view str{"Trains are fast!"};

    std::cout << str.length() << '\n';
    std::cout << str.substr(0, str.find(' ')) << '\n';
    std::cout << (str == "Trains are fast!") << '\n';

    // Since C++20
    std::cout << str.starts_with("Boats") << '\n';
    std::cout << str.ends_with("fast!") << '\n';

    std::cout << str << '\n';
}

void test_modify_string_view() {
    char arr[]{"Gold"};
    std::string_view str{arr};

    std::cout << str << '\n';  // print Gold
    arr[3] = 'f';

    std::cout << str << '\n';  // print Golf 
}

// change the view without modifying the viewed string.
void test_change_view() {
    std::string_view str{"Peach"};
    std::cout << str << '\n';

    str.remove_prefix(1);
    std::cout << str << '\n';  // each

    str.remove_suffix(2);
    std::cout << str << '\n';  // ea
}

// not use null terminators to mark the end of the string.
// Rather, it keeps track of string length.
void test_non_null_terminated() {
    char vowels[]{'a', 'e', 'i', 'o', 'u'};
    std::string_view str{vowels, std::size(vowels)};
    // vowels isn't null-terminated. We need to pass the length manually

    std::cout << str << '\n';  // This is safe.
}

// test ownership
std::string_view askForName() {
    std::cout << "What's your name?\n";
    std::string str{};  // internal string in dynamic memory
    std::cin >> str;

    std::string_view view{str};
    std::cout << "Hello " << view << '\n';
    return view;
}

void test_ownership() {
    std::string_view view{askForName()};
    std::cout << "Your name is " << view << '\n';  // undefined behavior (std::string 'str' died)
}

// converting std::string_view to std::string
void print(std::string s) {
    std::cout << s << '\n';
}

void test_convert_string_view_to() {
    std::string_view sv{"balloon"};

    // can't implictly convert
    // print(sv);  // compile error

    std::string str{sv};  // expensive, should be avoided if possible
    print(str);  // ok;

    print(static_cast<std::string>(sv));  // ok

    const char* szNullTerminated{str.c_str()};

    std::cout << str << " has " << std::strlen(szNullTerminated) << " letter(s)\n";

    // fast access to string being viewed(as a C-string) 
    // if view hasn't been modified (e.g. remove_prefix, remove_suffix)
    std::cout << std::strlen(str.data()) << '\n';
}

void test_incomplete_implementation() {
    std::string s{"hello"};
    std::string_view v{"world"};

    // Doesn't work
    // std::cout << (s + v) << '\n';
    // std::cout << (v + s) << '\n';

    // Potentially unsafe
    std::cout << (s + v.data()) << '\n';
    std::cout << (v.data() + s) << '\n';

    // Ok, but ugly and wasteful because we have to construct a new std::string
    std::cout << (s + std::string{v}) << '\n';
    std::cout << (std::string{v} + s) << '\n';
    std::cout << (s + static_cast<std::string>(v)) << '\n';
    std::cout << (static_cast<std::string>(v) + s) << '\n';
}


int main() {
    test_intro_string_view();
    test_string_functions();
    test_modify_string_view();
    test_change_view();
    test_non_null_terminated();
    test_ownership();

    test_convert_string_view_to();


    return 0;
}