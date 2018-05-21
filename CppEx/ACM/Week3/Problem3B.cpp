/**
 * Problem 3B: Expanding Fractions
 * familar with Problem 3A
 */

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <vector>
#include <map>

using namespace std;

int main(void) {
    string line;
    // ! this input still exists flaws: if there has some empty lines or some lines that
    // ! does not have two numbers.
    while (getline(cin, line)) {
        stringstream is(line);
        // 3000 >= numerator >= 0, 3000 >= denominator >= 1
        // ! no more extra check
        size_t numerator = 0, denominator = 1;
        // * numerator < denominator, denominator < 1000
        is >> numerator >> denominator;
        if (numerator == 0 && denominator == 0) {
            // each input instance should be followed by a blank line(include 0 0 terminal input)
            // cout << "\n";
            break;
        }
        
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
        cout << ".";
        count = 1;
        for (int i = 1; i < fraction.size(); i++) {
            // bit width strict to 50(include decimal point)
            if ((i - 1) % 50 == 49)
                printf("\n");
            printf("%d", fraction[i]);
        }
        
        if (cycle_pre == 0)
            printf("\nThis expansion terminates.\n\n");
        else
            printf("\nThe last %d digits repeat forever.\n\n", (fraction.size() - cycle_pre));
    }

    return 0;
}