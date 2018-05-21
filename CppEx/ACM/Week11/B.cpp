/**
 * B: Master-Mind Hints
 * 题目讲的太不清楚了, 就是猜数字, 如果相同位置猜对了(也就是 strong match), 那么这个数字在别的地方
 * 猜对了 weak match 也不用管了(不过 strong match 还是算的), 
 * 而且如果有同一个位置上的数字 weak match 了好几次, 只是按一次来算.
 * 
 * 循环, 排序查找
 * 
 * 需要注意的地方: 数组在不同迭代下如果在上一次被改变了, 下一次一定要还原回来
 * 读题很他妈的坑, 题目有些地方没讲清楚
 * 给定了范围的, 可以直接按照范围来创建数组, 例如 found
 * 
 * 对于数字范围比较小的, 大多可以直接开辟一个这么大的数组保存所有可能值, 然后暴力计算是否正确.
 */
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;

int main(int argc, char const *argv[])
{
    // the length of secret code, N <= 1000
    int N = 0;
    int cnt = 0;
    while (cin >> N && N != 0)
    {
        cnt++;
        cout << "Game " << cnt << ":\n";

        // code/guess \in [1,9]
        int codes[N] = {};
        int guesses[N] = {};

        // found[i] 为 true 表示数字 i + 1 已经剩余可以匹配的数量
        int foundRaw[9] = {0};

        for (int i = 0; i < N; i++)
        {
            cin >> codes[i];
            foundRaw[codes[i] - 1] += 1;
        }

        while (1)
        {
            // strongs[i] 表示位置 i 已经是 strong match 了
            bool strongs[N] = {0};
            for (int i = 0; i < N; i++)
            {
                cin >> guesses[i];
            }
            int codesSorted[N] = {0};
            int found[9];
            copy(foundRaw, foundRaw + 9, found);
            // copy
            copy(codes, codes + N, codesSorted);
            // sort
            sort(codesSorted, codesSorted + N);

            // check
            bool f = 0;
            int strong = 0, weak = 0;
            for (int i = 0; i < N; i++)
            {
                if (codes[i] == guesses[i])
                {
                    strong += 1;
                    found[guesses[i] - 1]--;
                    strongs[i] = 1;
                }
                if (guesses[i])
                {
                    f = 1;
                }
            }
            if (!f)
                break;
            // weak matchs
            for (int i = 0; i < N; i++)
            {
                if (!strongs[i] && found[guesses[i] - 1] && 
                    binary_search(codesSorted, codesSorted + N, guesses[i]))
                {
                    weak += 1;
                    found[guesses[i] - 1]--;
                }
            }
            // 如果 codes 中某个元素已经有 strong 了, 则不会有其他位置上的 guess 来作为 weak 的.
            // output
            cout << "    (" << strong << "," << weak << ")\n";
        }
    }

    return 0;
}
