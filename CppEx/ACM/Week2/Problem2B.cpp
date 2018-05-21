/**
* Problem2B
*/

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm> // reverse
#include <cmath> // pow
#include <cstdio> // printf

using namespace std;

const string DIGITS = "0123456789ABCDEF";

/**
 * 把任意进制(2 ~ 16)转化为十进制
 * @param number 按照题意
 * @param base 2 ~ 16 不做检查
 */
int convertto10(string &number, size_t base) {
    int decimal = 0, exp = 0; // exp 为指数, decimal 为结果
    reverse(number.begin(), number.end());
    for (char c : number) {
        decimal += DIGITS.find(c) * pow(base, exp++);
    }
    return decimal;
}

/**
 * 把十进制数转化为任意(2 ~ 16)进制
 * @param number 一定是正数, 这里懒得去处理负数了.
 */
string convert10to(int number, size_t base, string to = "", string bak = "") {
    return ((number / base == 0 && (((int)(bak = to).length()) > -1)) ? "" : convert10to(number / base, base, to))
        + string(1, DIGITS[number % base]) + bak;
}

int main(void) {
    string line;
    while (getline(cin, line)) {
        stringstream is(line);
        string baseNumber;
        size_t baseFrom = 0, baseTo = 0;
        is >> baseNumber >> baseFrom >> baseTo;

        // string d = "FF";
        // cout << "debug: " << convertto10(d, 16) << endl;

        string result = ::convert10to(::convertto10(baseNumber, baseFrom), baseTo);
        if (result.length() > 7)
            result = "ERROR";
        printf("%7s\n", result.c_str());
    }

    return 0;
}