/**
* A: STEED Cards
* scheme sketch: using STL algorithm library function: next_permutation
*/

#include <algorithm>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

/**
 * exmple implementations of STL algorithm permutation
 * ! be carful: must use iter_swap and reverse
 */
template<class Itr>
bool permutation(Itr first, Itr last) {
    Itr i = last;
    // check length >= 2
    if (first == last || first == --i)
        return false;
    
    for (Itr i2 = i; i != first; i2 = i) {
        if (*(--i) < *i2) {
            // found the first pair of numbers that left(i) < right(i2) for last to first.
            // e.g. 1 2 6 (6 8) 7 5 3
            // then swap left with the smallest number of numbers just visited in the right that large than
            // left
            Itr i3 = last;
            while (*(--i3) <= *i)
                continue;

            iter_swap(i3, i);

            // reverse right(i2) to last
            reverse(i2, last);
            return true;
        }
    }

    return false;
}

int main(void) {

    string word = "STEED";
    sort(word.begin(), word.end());
    vector<string> pers;
    do {
        pers.push_back(word);
    } while (permutation(word.begin(), word.end()));

    // * 1 <= n <= 60
    size_t n = 0;
    while (cin >> n) {
        cout << pers[n - 1] << "\n";
    }
}