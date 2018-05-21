/**
 * Problem 2A
 */

#include <iostream>
#include <bitset>
#include <string>

using namespace std;

int main(void) {
    int num = 0;
    cin >> num;

    while (num--) {
        int op1 = 0, op2 = 0;
        string op;
        cin >> hex >> op1 >> op >> op2;

        // output the results
        cout << bitset<13>(op1) << ' ' << op << ' ' << bitset<13>(op2) << " = " << 
            (op == "+" ? op1 + op2 : op1 - op2) << "\n";
    }

    // pause
    cin.get();
    cout << "\npress any key to continue...\n";
    cin.get();

    return 0;
}