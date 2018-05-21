/**
* Problem 4E: The Collatz Sequence
* scheme sketch: simulation but there are *TWO* cases shoud be handled carefully.
*/

// ! Fuck Uva, using scanf and printf instead of cin and cout.
#include <cstdio>
#include <iostream>

using namespace std;

int main(void) {
    int a = 0, L = 0, cnt = 0;
    // * limilations: a < L, 0 < a <= MAX_INT, 0 < L <= MAX_INT
    while (scanf("%d%d", &a, &L) == 2 && a > 0 && L > 0) {
        // int max = cycle_length(i);
        long len = 2L;
        // ! t may be large than MAX_INT
        long t = a;
        // ! handle t == 1
        // ! if t == 1, then 1, 4, 2, 1
        if(t % 2 == 0)  
            t /= 2;  
        else  
            t = 3 * t + 1; 
        while (t != 1) {
            if (t > L){
                len--;
                break;
            }
            if (t % 2)
                t = 3 * t + 1;
            else
                t = t / 2;
            len++;
        }
        printf("Case %d: A = %d, limit = %d, number of terms = %ld\n", ++cnt, a, L, len);
    }

    return 0;
}