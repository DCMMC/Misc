/**
* Problem 4C: The 3n + 1 problem
* scheme sketch: simulation
*/

// ! Fuck Uva, using scanf and printf instead of cin and cout.
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>

using namespace std;

int cycle_length(int n) {
    if (n == 1)
        return 1;
    else
        return 1 + cycle_length(n % 2 ? 3 * n + 1 : n / 2);
}

int main(void) {
    int m = 0, n = 0;
    while (scanf("%d%d", &m, &n) != EOF && m > 0 && n > 0) {
        // int max = cycle_length(i);
        int max = 0;
        int i = m, j = n;
        // ! i may be large than j
        // ! but must store previous m and n
        if (i > j) {
            int tmp = i;
            i = j;
            j = tmp;
        }
        for (int k = i; k <= j; k++) {
            // int len = cycle_length(k);
            int len = 1;
            int t = k;
            while (t - 1) {
                if (t % 2)
                    t = 3 * t + 1;
                else
                    t = t / 2;
                len++;
            }
            if (len > max)
                max = len;
        }
        printf("%d %d %d\n", m, n, max);
    }

    return 0;
}