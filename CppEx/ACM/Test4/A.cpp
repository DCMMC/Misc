/**
 * DHU Machine
 * 
 * // brute force simulate(m too large)
 * 
 * 
 * ! 位操作, 毕竟 m 顶多 30 位, 可以用 2bit 表示一个 bulb 的状态, 但是只有 3 个状态,
 * ! 所以看起来不能用位操作了, 那就用类似于处理 bitint 的链表
 * ! 好吧这种模拟太蠢了, 会 TLE
 * 
 * * 进制, 位运算
 * 
 * ! 反向思考, 按钮次数 n 就像是三进制加法, 按 3 次才会进一次位, 所以只需要把 n 转换成 3 进制就是答案了...
 * 
 * ! 这么大的数字 n, 一定要想到进制转换去
 * 
 * ! 注意是用 long long 还是 long double
 * 
 * ! 记住几个常用类型的常见大小(32bit/64bit 机器)
 */

#include <iostream>
#include <cstdio>

using namespace std;

int main(int argc, char const *argv[])
{
    int N;
    cin >> N;
    while (N--)
    {
        // [2, 30]
        int m;
        // [1, 2^63]
        unsigned long long n;
        cin >> m >> n;
        
        int bulbs[30];
        for (int i = 0; i < m; i++)
        {
            bulbs[i] = n % 3;
            n /= 3;
        }
        for (int i = m - 1; i >= 0; i--)
        {
            switch (bulbs[i])
            {
                case 0: cout << 'R'; break;
                case 1: cout << 'G'; break;
                case 2: cout << 'B'; break;
            }
        }

        cout << "\n";
    }

    return 0;
}
