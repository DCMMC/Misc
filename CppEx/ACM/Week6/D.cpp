/**
 * D: Power Crisis
 * skect scheme
 *      brute force, math/simulation
 */

#include <iostream>
#include <cstdio>

using namespace std;

int main(void) {
    // 13 <= n < 100
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
            for (int i = 2; i < n; i++) {
                index = (index - 1 + m) % i + 1;
            }
            // 这时候的 index 就是最后幸存下来的那个人的编号(编号从 1 开始)
            // 因为 1 号不算, 不计入约瑟夫环游戏
            if (index == 12) {
                cout << m << endl;
                break;
            }
        }
    }

    return 0;
}