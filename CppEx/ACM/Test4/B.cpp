/**
 * B Matrix
 * 
 * simulate?
 * 
 * 暴力一定会超时
 * 
 * ! 类似与空间换时间, lazy 计算, 先把所有操作记录在数组中, 最后输出的时候再操作
 * 
 * ! 对于这种时间敏感的题目, 用 printf 和 scanf 比 cin 和 cout 要快! 就是 AC 和 TLE 的差距!
 * ! 所以要一下所有常用类型的 scanf 格式说明符(long long, unsigned char 之类的)
 */

// #include <iostream>
#include <cstdio>

using namespace std;

int main(int argc, char const *argv[])
{
    int N;
    scanf("%d", &N);
    // cin >> N;
    while (N--)
    {
        int n, m, q;
        scanf("%d%d%d", &n, &m, &q);
        // cin >> n >> m >> q;
        int matrix[n][m] = { { 0 } };
        // 列映射, 简化操作 复杂度
        int map[m] = { 0 };
        // 行映射
        int mapRow[n] = { 0 };
        // ! 用数组记录增加的数而不是循环
        int addRow[n] = { 0 };
        int addCol[m] = { 0 };
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
            {
                // cin >> matrix[i][j];
                scanf("%d", &(matrix[i][j]));
                map[j] = j;
                mapRow[i] = i;
            }
        
        for (int i = 0; i < q; i++)
        {
            int z, x, y;
            scanf("%d%d%d", &z, &x, &y);
            // cin >> z >> x >> y;
            int t = 0;
            x--;
            switch (z) {
                case 1:
                    y--;
                    t = mapRow[x];
                    mapRow[x] = mapRow[y];
                    mapRow[y] = t;
                    break;
                case 2:
                    y--;
                    t = map[x];
                    map[x] = map[y];
                    map[y] = t;
                    break;
                case 3:
                    addRow[mapRow[x]] += y;
                    break;
                case 4:
                    addCol[map[x]] += y;
                    break;
            }
        }

        for (int i = 0; i < n; i++)
        {
            printf("%d", matrix[mapRow[i]][map[0]] + addRow[mapRow[i]] + addCol[map[0]]);
            // cout << matrix[mapRow[i]][map[0]];
            for (int j = 1; j < m; j++)
            {
                printf(" %d", matrix[mapRow[i]][map[j]] + addRow[mapRow[i]] + addCol[map[j]]);
                // cout << " " << matrix[mapRow[i]][map[j]];
            }
            printf("\n");
            // cout << "\n";
        }
    }
    return 0;
}

