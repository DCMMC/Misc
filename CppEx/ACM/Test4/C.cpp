/**
 * C: String
 * 
 * simulate
 * 
 * O(n^2) 暴力遍历 TLE 了...
 * 
 * ! 采用尺取法, 比原来的 i j 二重循环遇到恰好 k 个字母就加 length - j 要在优化一点
 * 
 * ! 这道题花了很多时间, 主要还是思路上没想到, 更加是不够细心, 写这种题太容易因为小问题出错了, 而且很难 debug
 */

#include <cstring>
#include <cstdio>

using namespace std;

char s[1000001];

int main(void)
{
    int N;
    scanf("%d", &N);
    while (N--)
    {
        int letters[26] = {0};
        int k;
        scanf("%s%d", s, &k);
        long long sumCnt = 0;
        memset(letters, 0, sizeof(letters));
        int len = strlen(s);
        int kCnt = 0;
        // ! 写这种 for 循环的时候一定要细心 !
        int i = 0, j = 0;
        while (i <= j)
        {
            // 先找到尺子的右边
            while (kCnt < k && j < len)
            {
                if (!letters[s[j] - 'a'])
                    kCnt++;
                letters[s[j] - 'a']++;
                j++;
            }

            // if (j >= len)
                // break;

            if (kCnt == k)
                // ! 这里一定要记得 + 1因为已经右移了一格!
                sumCnt += (len - j + 1);
            else if (j >= len)
                // ! 有可能出现 kCnt == k 并且 j == len 的情况, 所以一定要像判断 kCnt 是否等于 k 再去管 j
                // ! 这点太容易搞错了...
                break;

            letters[s[i] - 'a']--;
            if (letters[s[i] - 'a'] == 0)
            {
                kCnt--;
            }
            i++;
        } 

        printf("%lld\n", sumCnt);
    }

    return 0;
}
