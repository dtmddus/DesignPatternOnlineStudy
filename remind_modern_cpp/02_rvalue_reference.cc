#include <iostream>
#include <string>
using namespace std;

void test_call_by_value(const string s) {
    string str = s;     
    cout << "call by value" << endl;
}

void test(const string& s) {
    string str = s;  
    cout << "call by reference" << endl;
}

void test(const string&& s) {
    string str = s;  
    cout << "call by const rvalue reference" << endl;
}

void test_call_by_const_rvalue_reference(const string&& s) {
    string str = move(s);
    cout << "call by const rvalue reference" << endl;
}

void test_call_by_rvalue_reference(string&& s) {
    string str = move(s);
    cout << "call by rvalue reference" << endl;
}

int main() {
    string a = "abc";

    cout << "1. call by value" << endl;
    test_call_by_value(a);                 
    test_call_by_value("abc");    
    cout << endl;

    cout << "2. lvalue reference vs. rvalue reference" << endl;
    test(a);    
    test(move(a));
    test(static_cast<string&&>(a));
    test("abc");
    cout << endl;
        
    cout << "3. const rvalue reference vs. rvalue reference" << endl;
    test_call_by_const_rvalue_reference(move(a));  
    cout << a << endl;  // print "abc"
    test_call_by_rvalue_reference(move(a));  
    cout << a << endl;  // print ""
    cout << endl;

    return 0;
}