/**
* E: Eeny Meeny Moo
* skect scheme:
*       total same as D.cpp
*/

#include <iostream>

using namespace std;

int main(void) {
    // 3 <= n < 150
    int n = 0;
    while (cin >> n && n > 0) {
        int m = 0;
        while (1) {
            // ! m++ 一定不能为了简约放在 while 里面作为 while (1 || m++), g++ 跟 java 有点像,
            // ! 这样的情况不会继续判断后面的表达式了, 优化的时候会直接被删除, 所以副作用也没了...
            m++;
            // 从 1 开始依次尝试各种 m, 直到符合条件
            int index = 1;
            // 一定要记得这里排除了 1 号, 所以整个环的人数为 n - 1, 并且编号全部左移了一位
            for (int i = 2; i < n;) {
                // index = (index - 1 + m) % i + 1;
                // i++;
                // 这里有一个优化版本, 当 m < n 的时候, 考虑报数过程又回到第一个人时, 这时候有 floor(n/m) 死了
                // 例如 n = 8, m = 3
                // 1 2 3 4 5 6 7 8
                // 连续死了两个人后(用 x 表示死了的人的位置)
                // 1 2 x 4 5 x 7 8
                // 这时候重新编号:
                // 3 4 x 5 6 x 1 2
                // 这样一轮就可以杀好几个, 快很多, n 变成 n - n / m
                // O(m + log_{m/(m-1)}{n/m})
                
            }
            // 这时候的 index 就是最后幸存下来的那个人的编号(编号从 1 开始)
            // 因为 1 号不算, 不计入约瑟夫环游戏
            if (index == 1) {
                cout << m << endl;
                break;
            }
        }
    }

    return 0;
}