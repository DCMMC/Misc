/**
* Problem B: Overflow
* P.S. what is the exact overflow maximum integer number which
* is vary on different machine? 
*/

#include <string>
#include <iostream>
#include <limits> // int maximum
#include <regex> // regex_token_iterator and regex

using namespace std;

const int IMAX = numeric_limits<int>::max();

int main(void) {
	const string infos[] = {"first number too big",
		"second number too big",
		"result too big"};
	
	string line;
	const regex_token_iterator<string::iterator> regexSplitEnd;
	while (getline(cin, line)) {
		bool tooBigs[3] = {0}; // they present first, second, sum.
		long nums[2] = {0LL}; // they present first and second operands.
		// split
		size_t i = 0;
		// extra check
		//if (line.empty())
		//	continue;
		
		// https://stackoverflow.com/questions/32164501/er
		// ror-use-of-deleted-function-bool-regex-match-with-gcc-5-2-0
		// 2332. regex_token_iterator should forbid temporary regexes and temp strings
		string regStr("\\s*[*+]\\s*");
		regex reg(regStr);
		for (auto itr = regex_token_iterator<string::iterator>(line.begin(),
					line.end(), reg, -1); itr != regexSplitEnd; itr++, i++) {
			// to avoid num greater than long maximum, parses digits char by char instead.
			for (auto &c : string(*itr)) {
				if (nums[i] > IMAX) {
					tooBigs[i] = true;
					break;
				}
				nums[i] *= 10;
				nums[i] += (c - '0');
			}
		}
		
		// determine add/multiple overflow if two operands are both not overflow.
		if (!tooBigs[0] && !tooBigs[1]) {
			if (line.find("*")) {
				// determine multiple overflow
				if (nums[0] > IMAX / nums[1]) {
					tooBigs[2] = true;
				}
			} else {
				// determine addition overflow
				if (nums[0] > IMAX - nums[1])
					tooBigs[2] = true;
			}
		} else {
			tooBigs[2] = true;
		}
		
		// output the results
		cout << line << "\n";
		for (int i = 0; i != 3; i++) {
			if (tooBigs[i])
				cout << infos[i] << "\n";
		}	
	}
	
	// wait
    cout << '\n' << "Press the Enter key to continue.";
   	
	return 0;
}
