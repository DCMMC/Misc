/**
* Problem 4B: Uniform Generator
* seed(x + 1) = [seed(x) + STEP]%MOD
* scheme sketch: select proper STEP & MOD
*/

// ! Fuck Uva, using scanf and printf instead of cin and cout.
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>

using namespace std;

int main(void) {
    int s = 0, m = 0;
    char nums[100001];
    while (scanf("%d%d", &s, &m) == 2) {
        for (int i = 0; i < 100001; i++)
            nums[i] = 0;
        int seed = 0;
        while (!nums[seed]) {
            nums[seed] = 1;
            seed = (seed + s) % m;
        }
        for (int i = 0; i < m; i++) {
            if (!nums[i]) {
                // bad
                printf("%10d%10d%4sBad Choice\n\n", s, m, " ");
                seed = -1;
                break;
            }
        }
        if (seed != -1)
            printf("%10d%10d%4sGood Choice\n\n", s, m, " ");
    }

    return 0;
}


