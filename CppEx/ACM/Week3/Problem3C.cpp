/**
* Problem 3C: Rational Numbers from Repeating Fractions
* scheme sketch: shift bits accordig to non repetend part and using Eculid GCD
* to simplify fraction
*/

// ! Fuck Uva, using scanf and printf instead of cin and cout.
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

/**
 * Eculid GCD
 */
long long gcd(long long p, long long q) {
	if (p == 0 || q == 0)
		return q == 0 ? 1 : q;
	
    p = abs(p);
    q = abs(q);

    // |p| >= |q|
    if (p < q) {
        long long tmp = p;
        p = q;
        q = tmp;
    }

    if (p % q == 0)
        return q;
    else
        return gcd(q, p % q);
}

int main(void) {
	// ! some interest test cases: 0 0, 0 1.5, 2 .33, 1 -0.3, 2 25.318
	
    // index in the test case
    int indexCase = 1;
    // j indicate how many digits that repeat in the end.
    int j = 0;
    // * digit's number in decimal expansion exceeds 9
    // ! using cin and string can fit more situations(any decimal expansion length).
    // there is no ignore if integer part or fractional part is 0
    char repeatingFraction[1001];
    while (scanf("%d", &j) == 1 && j != -1) {
        scanf("%s", repeatingFraction);
        string s(repeatingFraction);
        // add 0 repetend part to finate fraction
        if (j == 0) {
            j = 1;
            // handle the case that fractional part is 0 and be hided.
            s += s.find('.') != s.npos ? "0" : ".0";
        }

		// handle +/- sign 
        bool negative = false;
        if (s[0] == '-') {
            negative = true;
            s.erase(0, 1);
        } else if (s[0] == '+') {
            s.erase(0, 1);
        } else if (s[0] == '.') {
        	// handle if integer part is 0 and be ignored.
        	// such as .314
        	s = "0" + s; 
		}
        
        // * onePeriod is the integer part of repeatingFraction * 10 ^ (multipleOne)
        // * secondPeriod is the integer part of repeatingFraction * 10 ^ (multipleSecond)
        size_t multipleOne = s.size() - 1 - j - s.find('.');
        size_t multipleSecond = multipleOne + j;
//        cout << "debug: j = " << j << ", s = " << s << endl;
//        cout << "debug: multipleOne = " << multipleOne << ", multipleSecond = " << multipleSecond << endl;
        // t is the non repetend part(exclude decimal point)
		string t = s.substr(0, s.size() - j);
        t.erase(t.begin() + t.find('.'));
//        cout << "debug: t = " << t << endl;
        long long onePeriod = atoll(t.c_str());
        long long secondPeriod = onePeriod * pow(10, j) + atoll(s.substr(s.size() - j, s.size() - 1).c_str());
        // (10 ^ multipleSecond - 10 ^ multipleOne) * number = (secondPeriod - onePeriod)
        long long numerator = secondPeriod - onePeriod;
        long long denominator = pow(10, multipleSecond) - pow(10, multipleOne);
        long long gcdV = gcd(numerator, denominator);
//        cout << "debug: onePeriod = " << onePeriod << ", secondPeriod = " << secondPeriod <<
//            ", numerator = " << numerator << ", denominator = " << denominator << ", gcdV = "
//            << gcdV << endl;
        numerator /= gcdV;
        denominator /= gcdV;

        // output
        printf("Case %d: %lld/%lld\n", indexCase++, negative ? -numerator : numerator, denominator);
    }

    return 0;
}
