/**
* Problem 3D: Perfection
* a perfect number is a positive integer that is equal to the sum of all its positive, proper
* divisors. e.g. 6 = 1 + 2 + 3
* Analogously, deficient indicates smaller than the sum, abundant indicates larger than the sum.
*/

// ! Fuck Uva, using scanf and printf instead of cin and cout.
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

int main(void) {
	// ! interesting test cases: 0
	// * input: a list of N positive integers(<= 60,000, and 1 < N < 100) 
	size_t num = 0;
	vector<size_t> list;
	while ( ((bool)(cin >> num)) && num > 0) {
		list.push_back(num); 
	}
	
	printf("PERFECTION OUTPUT\n");
	for (const auto i : list) {
		size_t divisorS = 0;
		for (size_t d = 1; d < i; d++) {
			if (i % d == 0)
				divisorS += d;
		}
		printf("%5lld  %s\n", i, divisorS == i ? "PERFECT" : (divisorS < i ? "DEFICIENT" : "ABUNDANT"));
	}
	printf("END OF OUTPUT\n");
	
	return 0;
}
