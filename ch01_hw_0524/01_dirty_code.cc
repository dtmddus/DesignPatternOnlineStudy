#include <iostream>
using namespace std;

int dirty_code(int a, int b, char op) {
    int res;
    switch (op) {
        case '+': {
            res = a + b;
            break;
        }
        case '-': {
            res = a - b;
            break;
        }
        case '*': {
            res = a * b;
            break;
        }
        case '/': {
            res = a / b;
            break;
        }
    }
    return res;
}

int main() {
    for (int i = 0; i < 4; i++) {
        int a;
        int b;
        char op;
        cin >> a >> b >> op;
        cout << "a : " << a << endl;
        cout << "b : " << b << endl;
        cout << "op: " << op << endl;
        cout << "Result: " << dirty_code(a, b, op) << endl;
    }
    return 0;
}