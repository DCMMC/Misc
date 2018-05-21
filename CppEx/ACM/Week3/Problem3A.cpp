/**
 * Problem 3A: Repeating Decimals
 * scheme sketch: simulate long division
 * * @since C++ 11
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

int main(void) {
    // some interesting datas
    // 3000 / 1, 0 / 1, 0 / 3000, 2999 / 3000, 1 / 2, 1 / 14, 1 / 397, 1 / 3000, 199 / 1985
    string line;
    // ! this input still exists flaws: if there has some empty lines or some lines that
    // ! does not have two numbers.
    while (getline(cin, line)) {
        stringstream is(line);
        // 3000 >= numerator >= 0, 3000 >= denominator >= 1
        // ! no more extra check
        size_t numerator = 0, denominator = 1;
        is >> numerator >> denominator;
        size_t remainNumerator = numerator;

        // calculate
        vector<unsigned> fraction;
        // integer part
        fraction.push_back(remainNumerator / denominator);
        remainNumerator = (remainNumerator % denominator) * 10;
        map<unsigned, size_t> remainders;
        // cycle_pre indicates the index of first element of repetend cycle
        // `0` show that no repetend cycle.
        size_t count = 1, cycle_pre = 0;
        
        do {
            // O(logN)
            remainders.insert(pair<unsigned, size_t>(remainNumerator, count++));
            fraction.push_back(remainNumerator / denominator);
            remainNumerator = (remainNumerator % denominator) * 10;

            // O(logN)
            auto itr = remainders.find(remainNumerator);
            if (itr != remainders.end()) {
                // found repetend cycle
                cycle_pre = itr->second;
                break;
            }
        } while(remainNumerator != 0);

        // output
        if (cycle_pre == 0) {
            // no repetend cycle
            fraction.push_back(0);
            cycle_pre = fraction.size() - 1;
        }
        cout << numerator << "/" << denominator << " = " << fraction[0] << ".";
        for (size_t i = 1; i < cycle_pre; i++) {
            cout << fraction[i];
        }
        cout << "(";
        for (size_t i = cycle_pre; i < (fraction.size() - cycle_pre > 50 ? 50 + cycle_pre : fraction.size());
                i++) {
            cout << fraction[i];
        }
        if (fraction.size() > 51)
            cout << "...";
        cout << ")" << endl << "   " << fraction.size() - cycle_pre << 
            " = number of digits in repeating cycle\n"
            << endl;
    }
    
    return 0;
}