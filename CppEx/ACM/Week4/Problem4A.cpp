/**
* Problem 4A: Pseudo-Random Numbers
* scheme sketch: Next = (Z * Last + I) mod M
*/

// ! Fuck Uva, using scanf and printf instead of cin and cout.
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>

using namespace std;

int main(void) {
    // case count
    size_t c = 1;
    int Z = 0, I = 0, M = 1, L = 0;
    while (scanf("%d%d%d%d", &Z, &I, &M, &L) == 4 && !(Z == 0 && I == 0 && M == 0 && L == 0)) {
        int L_bak = L;
        // ! the cycle might not begin with the seed, so store all numbers in repeat cycle.
        int nums[10000];
        for (int i = 0; i < 10000; i++)
            nums[i] = 0;

        // repeat count
        int r = 0;
        while (!(nums[L])) {
            nums[L] = ++r;
            L = (Z*L + I) % M;
        }
        
        // ! the cycle might not begin with the seed, i.e. nums[L]
        printf("Case %d: %d\n", c++, r - nums[L] + 1);
    }

    return 0;
}