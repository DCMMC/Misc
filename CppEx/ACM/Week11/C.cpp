/**
 * Linear Cellular Automata
 * 
 * !  记得第一天是初始的那个 DNA, 所以后面只需要遍历 49 天!
 * !  这个一定要记清楚!!!
 * smulate
 */

#include <iostream>

using namespace std;

void p(int DNA[]) {
    for (int i = 1; i <= 40; i++) {
        switch (DNA[i]) {
            case 0: cout << ' '; break;
            case 1: cout << '.'; break;
            case 2: cout << "x"; break;
            case 3: cout << 'W'; break;
        }
    }
    cout << "\n";
}

int main(int argc, char const *argv[])
{
    // num of test cases
    int N;
    cin >> N;
    int DNA_prog[10];

    for (int i = 0; i < 10; i++) {
            cin >> DNA_prog[i];
        }
        // 左右两个是边界假想点, 都为 0
        int DNA[42] = {0};
        // population density
        // 0: ' ', 1: '.', 2: 'x', 3: 'W'
        DNA[20] = 1;
        p(DNA);
        // 50 - 1 days
        // !  记得第一天是初始的那个 DNA, 所以后面只需要遍历 49 天!
        // !  这个一定要记清楚!!!
        int bak = 0;
        for (int i = 0; i < 49; i++) {
            bak = 0;
            for (int j = 1; j <= 40; j++) {
                int tmp = DNA[j];
                DNA[j] = DNA_prog[bak + DNA[j] + DNA[j + 1]];
                bak = tmp;
            }
            // print
            p(DNA);
        }

    while (--N) {
        cout << "\n";

        for (int i = 0; i < 10; i++) {
            cin >> DNA_prog[i];
        }
        // 左右两个是边界假想点, 都为 0
        int DNA[42] = {0};
        // population density
        // 0: ' ', 1: '.', 2: 'x', 3: 'W'
        DNA[20] = 1;
        p(DNA);
        // 50 - 1 days
        // !  记得第一天是初始的那个 DNA, 所以后面只需要遍历 49 天!
        // !  这个一定要记清楚!!!
        int bak = 0;
        for (int i = 0; i < 49; i++) {
            bak = 0;
            for (int j = 1; j <= 40; j++) {
                int tmp = DNA[j];
                DNA[j] = DNA_prog[bak + DNA[j] + DNA[j + 1]];
                bak = tmp;
            }
            // print
            p(DNA);
        }
    }

    return 0;
}
