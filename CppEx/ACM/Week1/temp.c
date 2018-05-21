#include <stdio.h>

int main(void) {
    int m = 0, n = 0;
    while (scanf("%d%d", &m, &n) != EOF && m > 0 && n > 0) {
        unsigned long long max = 0ULL;
        int i = m, j = n;
        if (i > j) {
            int tmp = i;
            i = j;
            j = tmp;
        }
      	int k, v = 0;
        for (k = i; k <= j; k++) {
            unsigned long long len = 0ULL;
            int t = k;
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