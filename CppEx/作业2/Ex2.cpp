/**
 * Cpp exercise 2
 */

/**
 * some simple but useful utils, Fuck Cpp
 * * @author DCMMC
 * * @since C++ 11
 */
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include "utils.hpp"

using namespace std;

/**
 * 1. determine days according to input(yyyy-mm-dd)
 * * @param date fuck
 * * @return -1 indicates error date
 */
int days(const string &date) {
    // check validation
    regex re("\\d{4}\\-\\d{1,2}\\-\\d{1,2}");
    if (!regex_match(date, re))
        return -1;

    auto yearMonDay = utils::split(date, "-");

    // * judge leap year
    int year = stoi(yearMonDay[0]);
    bool febPlus1 = false;
    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
        febPlus1 = true;
    switch (stoi(yearMonDay[1])) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
            break;
        case 2:
            return febPlus1 ? 29 : 28;
            break;
    }

    return 0;
}

/**
 * 2. invert
 */
vector<int> invert(int A[], int n) {
    vector<int> v;
    while(n--) {
        v.push_back(A[n]);
    }
    return v;
}

// ! duplicated
// vector<int> &invert(int *A, int n) {
//     vector<int> v;
//     while(n--) {
//         v.push_back(A[n]);
//     }
//     return v;
// }


/**
 * 3. sort using reference
 */
vector<int> sort(int &a, int &b, int &c) {
    auto t = vector<int>();
    t.push_back(a);
    t.push_back(b);
    t.push_back(c);
    sort(begin(t), end(t));
    return t;
}

/**
 * 4. sort
 */
vector<int> sort(vector<int> &v) {
    sort(begin(v), end(v));
    return v;
}

vector<float> sort(vector<float> &v) {
    sort(begin(v), end(v));
    return v;
}

vector<double> sort(vector<double> &v) {
    sort(begin(v), end(v));
    return v;
}

int main(void) {
    // T1
    cout << "T1:\nInput: yyyy-mm-dd:\n";
    string input;
    cin >> input;
    cout << "days: " << days(input) << "\n";

    // T2
    cout << "\nT2\n";
    int a[] = {1, 2, 3, 4}, b[] = {1, 2, 3, 4};
    int *c = b;
    auto foo = [](vector<int> v) {
        for (auto i : v)
            cout << i << " ";
        cout << "\n";
    };
    foo(invert(a, 4));
    foo(invert(c, 4));

    // T3
    cout << "\nT3\n";
    int num0 = 3, num1 = 1, num2 = 6;
    auto t = sort(num0, num1, num2);
    for (auto i : t)
        cout << i << " ";
    cout << "\n";

    // T4
    cout << "\nT4\n";
    int myints[] = {16,2,77,29};
    auto v = vector<int>(myints, myints + sizeof(myints) / sizeof(int));
    foo(sort(v));

    // pause
    cin.get();
    cout << "\npress any key to continue...\n";
    cin.get();

    return 0;
}