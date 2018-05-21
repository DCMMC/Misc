/**
 * Problem2C
 */

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

const string DIGITS = "0123456789ABCDEF";

/**
 * 把任意进制(2 ~ 16)转化为十进制
 * @param number 按照题意
 * @param base 2 ~ 16 不做检查
 * @return 如果非法就返回 -1
 */
long long convertto10(string number, int base) {
    long long decimal = 0, exp = 0; // exp 为指数, decimal 为结果
    reverse(number.begin(), number.end());
    for (char c : number) {
        int index = DIGITS.find(c);
        if (index == string::npos || index >= base)
            return -1;
        decimal += index * pow(base, exp++);
    }
    return decimal;
}

/**
 * 把十进制数转化为任意(2 ~ 16)进制
 * @param number 一定是正数, 这里懒得去处理负数了.
 */
string convert10to(long long number, size_t base, string to = "", string bak = "") {
    return ((number / (long long)base == 0 && (((int)(bak = to).length()) > -1)) ? "" : convert10to(number / base, base, to))
        + string(1, DIGITS[number % (long long)base]) + bak;
}

int main(void) {
    char number[10];
    int baseFrom = 0, baseTo = 0;
    while (scanf("%d %d %s", &baseFrom, &baseTo, number) == 3) {
        int decimal = -1;
        if (baseFrom > 0 && baseFrom <= 16 && baseTo > 0 && baseTo <= 16)
            decimal = convertto10(string(number), baseFrom);
        if (decimal == -1)
            cout << number << " is an illegal base " << baseFrom << " number\n";
        else
            cout << number << " base " << baseFrom << " = " << convert10to(decimal, baseTo) << " base "
                << baseTo << "\n";
    }

    return 0;
}