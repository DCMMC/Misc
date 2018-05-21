/**
* Problem 3E: Simply Subsets
* some simple set relation operations 
*/

// ! Fuck Uva, using scanf and printf instead of cin and cout.
#include <iostream>
#include <string>
#include <sstream>
#include <set>

using namespace std;

int main(void) {
	string line;
	while (getline(cin, line)) {
		stringstream is(line);
		int num = 0;
		set<int> A;
		while (is >> num) {
			A.insert(num); 
		}
		getline(cin, line);
		stringstream iss(line);
		set<int> B;
		while (iss >> num) {
			B.insert(num); 
		}
		
		// compare
		bool found = false, notFound = false;
		if (A.size() < B.size()) {
			for (auto i : A) {
				if (B.find(i) == B.end())
					notFound = true;
				else
					found = true;
			}
			if (!found && notFound)
					cout << "A and B are disjoint\n";
				else if (found && !notFound)
					cout << "A is a proper subset of B\n";
				else
					cout << "I'm confused!\n";
		} else if (A.size() == B.size()) {
			if (A == B)
				cout << "A equals B\n";
			else {
				bool confused = false;
				for (auto i : A)
					if (B.find(i) != B.end()) {
						confused = true;
						break;
					}
				
				
				if (confused)
					cout << "I'm confused!\n";
				else
					cout << "A and B are disjoint\n";
			}
		} else {
			for (auto i : B) {
				if (A.find(i) == A.end())
					notFound = true;
				else
					found = true;
			}
			if (!found && notFound)
				cout << "A and B are disjoint\n";
			else if (found && !notFound)
				cout << "B is a proper subset of A\n";
			else
				cout << "I'm confused!\n";
		}
	}
	
	return 0;
}
