/**
* Problem 4D: Ackermann Functions
* scheme sketch: simulation
*/

// ! Fuck Uva, using scanf and printf instead of cin and cout.
#include <cstdio>
#include <iostream>

using namespace std;

int main(void) {
    int m = 0, n = 0;
    while (scanf("%ld%ld", &m, &n) != EOF && (m || n)) {
        // int max = cycle_length(i);
        unsigned long long max = 0, len, t;
        int i = m, j = n;
        // ! i may be large than j
        if (i > j) 
            swap(i, j);
        int v = i;
        for (int k = i; k <= j; k++) {
            // int len = cycle_length(k);
            len = 1;
            t = k;
            // ! handle t == 1
            // ! if t == 1, then 1, 4, 2, 1
            if(t % 2 == 0)  
                t /= 2;  
            else  
                t = 3 * t + 1; 
            while (t - 1) {
                if (t % 2)
                    t = 3 * t + 1;
                else
                    t = t / 2;
                len++;
            }
            if (len > max) {
                max = len;
                v = k;
            }
        }
        printf("Between %d and %d, %d generates the longest sequence of %lld values.\n", i, j, v, max);
    }

    return 0;
}