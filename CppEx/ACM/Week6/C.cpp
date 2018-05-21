/**
* C: Joseph
* scheme sketch:
*       brute force, simulation/math
*/

#include <cstdio>
#include <vector>
#include <iostream>

using namespace std;

/**
 * 预处理出答案, 到时候直接用
 * count start from 1
 */
void preprocess(int k = 13) {
    // 从 1 个坏人 1 个好人一直暴力到 13 个坏人 13 个好人
    for (int n = 1; n <= k; n++) {
        bool found = true;
        // m 尝试所有第一个出队列的人的编号在后半部分(也就是坏人)的可能的 m
        for (int m = n + 1; true; m % (2 * n) == 0 ? m += (n + 1) : m++) {
            vector<int> result;
            // 用模拟方法来模拟约瑟夫环问题需要 O(n*m) on time
            // 用数学公式来模拟解决约瑟夫环问题, O(n^2) on time (不过在求最后那个存活者的时候只要 O(n), 优于模拟)
            // 优化版本的可以到 O(n logn)
            // 很多的请参考:
            // http://maskray.me/blog/2013-08-27-josephus-problem-two-log-n-solutions#disqus_thread
            // f(n, m) = ( f(n - 1) + m ) mod n
            // 上述公式是用于从 0 开始编号的, 可以用数学归纳证明
            // 依次从第一个出圈的序号一直到最后一个出圈的序号
            // 这里 f(n) 表示 n 个人的圈中第一个出去的那个人的序号
            for(int i = n * 2; i > n; i--) {
                // 这里的 index 是 t 个人的圈, 从 1 开始编号, 循环报数第 m 个人的编号
                int t = i, index = (m - 1) % t + 1;
                // 这里使用的是动态规划(Dynamic Programming, abbr, DP)
                // 也就是每一步的结果都是在上一步的结果的基础上来的
                while (t++ < n * 2) {
                    // f(t + 1) = (f(t) + m) mod (t + 1)
                    // 上述公式是用于从 0 开始编号的
                    index = (index - 1 + m) % t + 1;
                }
                if (index <= n)
                {
                    // 如果抓了好人 (<= n)
                    found = false;
                    break;
                } else
                {
                    result.push_back(index);
                    found = true;
                }
            }
            if (found)
            {
                // printf("m = %d 时, 出队列顺序: (从 1 开始编号)\n", m);
                // for (auto idx : result)
                //     printf("%d, ", idx);
                // printf("\n\n");
                printf("%d, ", m);
                break;
            }
        }
    }
}

int main(void) {
    // ! 0 < k < 14, 对于这种规模比较小但是又 theory difficulty 比较 trivial 的问题, 比较适合暴力解决
    // preprocess();
    // 答案由上面的函数输出
    int results[] = {2, 7, 5, 30, 169, 441, 1872, 7632, 1740, 93313, 459901, 1358657, 2504881};
    int k = 0;
    while (cin >> k && k > 0)
        cout << results[k - 1] << endl;

    return 0;
}