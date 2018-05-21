/**
* Problem 2E
*/

#include <iostream>
#include <string>
#include <sstream> // istringstream
#include <vector>

using namespace std;

/**
 * @param a sorted array
 * @param key the element value to search
 * @template T key type
 * @return -1 if not found
 * 
 * Complexity
 * O(logN)
 */
template<typename T = int>
int binarySearch(const vector<T> &a, T key) {
    int lo = 0, hi = a.size() - 1, mid = -1;
    while (lo <= hi) {
        mid = (hi + lo) / 2;
        if (a[mid] < key)
            lo = mid + 1;
        else if (a[mid] > key)
            hi = mid - 1;
        else
            return mid;
    }
    return -1;
}

int main(void) {
	// 1 is *NOT* prime in math!
	const vector<int> primes = {1,    2,    3,    5,    7,   11,   13,   17,   19,   23,   29, 
	   31,   37,   41,   43,   47,   53,   59,   61,   67,   71, 
	   73,   79,   83,   89,   97,  101,  103,  107,  109,  113, 
	  127,  131,  137,  139,  149,  151,  157,  163,  167,  173, 
	  179,  181,  191,  193,  197,  199,  211,  223,  227,  229, 
	  233,  239,  241,  251,  257,  263,  269,  271,  277,  281, 
	  283,  293,  307,  311,  313,  317,  331,  337,  347,  349, 
	  353,  359,  367,  373,  379,  383,  389,  397,  401,  409, 
	  419,  421,  431,  433,  439,  443,  449,  457,  461,  463, 
	  467,  479,  487,  491,  499,  503,  509,  521,  523,  541, 
	  547,  557,  563,  569,  571,  577,  587,  593,  599,  601, 
	  607,  613,  617,  619,  631,  641,  643,  647,  653,  659, 
	  661,  673,  677,  683,  691,  701,  709,  719,  727,  733, 
	  739,  743,  751,  757,  761,  769,  773,  787,  797,  809, 
	  811,  821,  823,  827,  829,  839,  853,  857,  859,  863, 
	  877,  881,  883,  887,  907,  911,  919,  929,  937,  941, 
	  947,  953,  967,  971,  977,  983,  991,  997};
	
	string line;

	while (getline(cin, line)) {
		istringstream is(line);
		int n = 1, c = 0;
		is >> n >> c;
        int prime = n;

        int index = -1;
		// local maximum prime number that less than or equal to `n`
		while (prime--) {
			if ( (index = ::binarySearch<int>(primes, prime)) > -1 ) {
                // output the result
                cout << n << " " << c << ":";
                int beyondBak = 0;
                if (0 == index % 2) {
                    // list 为奇数个
                    for (int i = ((beyondBak = index / 2 - c + 1) < 0 ? 0 : beyondBak);
                            i <= ((beyondBak = index / 2 + c - 1) > index ? index : beyondBak); i++)
                        cout << " " << primes[i];
                } else {
                    // list 为偶数个
                    for (int i = ((beyondBak = index / 2 - c + 1) < 0 ? 0 : beyondBak);
                            i <= ((beyondBak = index / 2 + c) > index ? index : beyondBak); i++)
                        cout << " " << primes[i];
                }
                cout << "\n\n";
                break;
            }
		} 
	}
}
