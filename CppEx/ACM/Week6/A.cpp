/**
 * A: The Sultan's Successors
 * scheme sketch:
 *              先用 dfs 回溯递归(其实比较暴力)算出答案, 毕竟就 92 种, 然后直接查表就行
 */

#include <cstdio>

using namespace std;

#define QUEENS 8

// cols 表示列中是否有一个 Queen 了, diag 表示正对角线, reveDiag 表示反对角线
unsigned char cols[QUEENS] = {0}, diag[3 * (QUEENS - 1) + 1] = {0},
              reveDiag[3 * (QUEENS - 1) + 1] = {0},
              chess[QUEENS][QUEENS] = {0};
size_t count = 0;

/**
 * 8-Queue recursive function
 * depth-first search and backtrack
 * 这算是带有条件判断的暴力算法, 尝试 < 8! 种可能
 * @param i
 *         row index
 */
void lay(int i = 0)
{
    // col index
    int j = -1;
    while (++j < QUEENS)
    {
        if (cols[j] + diag[j - i + QUEENS - 1] + reveDiag[j + i + QUEENS - 1] == 0)
        {
            // 如果行, 列, 对角线都没有摆放 QUEEN, 就在 (i,j) 位置放置一个 QUEEN
            chess[i][j] = cols[j] = diag[j - i + QUEENS - 1] = reveDiag[j + i + QUEENS - 1] = 1;
            if (i < QUEENS - 1)
                // 在下一行选择放置 QUEEN
                // 这里的递归和回溯处理需要很仔细的思考
                lay(i + 1);
            else
            {
                // 输出
                count++;
                int t, u;
                // for (printf("(%ld)\n", count), u = QUEENS; --u; printf("\n"))
                //     for (t = QUEENS; --t;)
                //         chess[t][u] ? printf("Q ") : printf("+ ");
                // printf("\n\n\n");
                printf("\"");
                for (t = 0; t < QUEENS; t++)
                    for (u = 0; u < QUEENS; u++)
                        if (chess[u][t])
                            printf("%d", u);
                printf("\", \n");
            }
            // 结束之后一定要记得清零(回溯)
            chess[i][j] = cols[j] = diag[j - i + QUEENS - 1] = reveDiag[j + i + QUEENS - 1] = 0;
        }
    }
}

int main(void)
{
    lay();
    // hard code solutions by lay() - 92 种
    // 查表速度最快
    unsigned char solutions[92][17] = {
        {0, 0, 1, 4, 2, 7, 3, 5, 4, 2, 5, 6, 6, 1, 7, 3, 0},
        {0, 0, 1, 5, 2, 7, 3, 2, 4, 6, 5, 3, 6, 1, 7, 4, 0},
        {0, 0, 1, 6, 2, 3, 3, 5, 4, 7, 5, 1, 6, 4, 7, 2, 0},
        {0, 0, 1, 6, 2, 4, 3, 7, 4, 1, 5, 3, 6, 5, 7, 2, 0},
        {0, 1, 1, 3, 2, 5, 3, 7, 4, 2, 5, 0, 6, 6, 7, 4, 0},
        {0, 1, 1, 4, 2, 6, 3, 0, 4, 2, 5, 7, 6, 5, 7, 3, 0},
        {0, 1, 1, 4, 2, 6, 3, 3, 4, 0, 5, 7, 6, 5, 7, 2, 0},
        {0, 1, 1, 5, 2, 0, 3, 6, 4, 3, 5, 7, 6, 2, 7, 4, 0},
        {0, 1, 1, 5, 2, 7, 3, 2, 4, 0, 5, 3, 6, 6, 7, 4, 0},
        {0, 1, 1, 6, 2, 2, 3, 5, 4, 7, 5, 4, 6, 0, 7, 3, 0},
        {0, 1, 1, 6, 2, 4, 3, 7, 4, 0, 5, 3, 6, 5, 7, 2, 0},
        {0, 1, 1, 7, 2, 5, 3, 0, 4, 2, 5, 4, 6, 6, 7, 3, 0},
        {0, 2, 1, 0, 2, 6, 3, 4, 4, 7, 5, 1, 6, 3, 7, 5, 0},
        {0, 2, 1, 4, 2, 1, 3, 7, 4, 0, 5, 6, 6, 3, 7, 5, 0},
        {0, 2, 1, 4, 2, 1, 3, 7, 4, 5, 5, 3, 6, 6, 7, 0, 0},
        {0, 2, 1, 4, 2, 6, 3, 0, 4, 3, 5, 1, 6, 7, 7, 5, 0},
        {0, 2, 1, 4, 2, 7, 3, 3, 4, 0, 5, 6, 6, 1, 7, 5, 0},
        {0, 2, 1, 5, 2, 1, 3, 4, 4, 7, 5, 0, 6, 6, 7, 3, 0},
        {0, 2, 1, 5, 2, 1, 3, 6, 4, 0, 5, 3, 6, 7, 7, 4, 0},
        {0, 2, 1, 5, 2, 1, 3, 6, 4, 4, 5, 0, 6, 7, 7, 3, 0},
        {0, 2, 1, 5, 2, 3, 3, 0, 4, 7, 5, 4, 6, 6, 7, 1, 0},
        {0, 2, 1, 5, 2, 3, 3, 1, 4, 7, 5, 4, 6, 6, 7, 0, 0},
        {0, 2, 1, 5, 2, 7, 3, 0, 4, 3, 5, 6, 6, 4, 7, 1, 0},
        {0, 2, 1, 5, 2, 7, 3, 0, 4, 4, 5, 6, 6, 1, 7, 3, 0},
        {0, 2, 1, 5, 2, 7, 3, 1, 4, 3, 5, 0, 6, 6, 7, 4, 0},
        {0, 2, 1, 6, 2, 1, 3, 7, 4, 4, 5, 0, 6, 3, 7, 5, 0},
        {0, 2, 1, 6, 2, 1, 3, 7, 4, 5, 5, 3, 6, 0, 7, 4, 0},
        {0, 2, 1, 7, 2, 3, 3, 6, 4, 0, 5, 5, 6, 1, 7, 4, 0},
        {0, 3, 1, 0, 2, 4, 3, 7, 4, 1, 5, 6, 6, 2, 7, 5, 0},
        {0, 3, 1, 0, 2, 4, 3, 7, 4, 5, 5, 2, 6, 6, 7, 1, 0},
        {0, 3, 1, 1, 2, 4, 3, 7, 4, 5, 5, 0, 6, 2, 7, 6, 0},
        {0, 3, 1, 1, 2, 6, 3, 2, 4, 5, 5, 7, 6, 0, 7, 4, 0},
        {0, 3, 1, 1, 2, 6, 3, 2, 4, 5, 5, 7, 6, 4, 7, 0, 0},
        {0, 3, 1, 1, 2, 6, 3, 4, 4, 0, 5, 7, 6, 5, 7, 2, 0},
        {0, 3, 1, 1, 2, 7, 3, 4, 4, 6, 5, 0, 6, 2, 7, 5, 0},
        {0, 3, 1, 1, 2, 7, 3, 5, 4, 0, 5, 2, 6, 4, 7, 6, 0},
        {0, 3, 1, 5, 2, 0, 3, 4, 4, 1, 5, 7, 6, 2, 7, 6, 0},
        {0, 3, 1, 5, 2, 7, 3, 1, 4, 6, 5, 0, 6, 2, 7, 4, 0},
        {0, 3, 1, 5, 2, 7, 3, 2, 4, 0, 5, 6, 6, 4, 7, 1, 0},
        {0, 3, 1, 6, 2, 0, 3, 7, 4, 4, 5, 1, 6, 5, 7, 2, 0},
        {0, 3, 1, 6, 2, 2, 3, 7, 4, 1, 5, 4, 6, 0, 7, 5, 0},
        {0, 3, 1, 6, 2, 4, 3, 1, 4, 5, 5, 0, 6, 2, 7, 7, 0},
        {0, 3, 1, 6, 2, 4, 3, 2, 4, 0, 5, 5, 6, 7, 7, 1, 0},
        {0, 3, 1, 7, 2, 0, 3, 2, 4, 5, 5, 1, 6, 6, 7, 4, 0},
        {0, 3, 1, 7, 2, 0, 3, 4, 4, 6, 5, 1, 6, 5, 7, 2, 0},
        {0, 3, 1, 7, 2, 4, 3, 2, 4, 0, 5, 6, 6, 1, 7, 5, 0},
        {0, 4, 1, 0, 2, 3, 3, 5, 4, 7, 5, 1, 6, 6, 7, 2, 0},
        {0, 4, 1, 0, 2, 7, 3, 3, 4, 1, 5, 6, 6, 2, 7, 5, 0},
        {0, 4, 1, 0, 2, 7, 3, 5, 4, 2, 5, 6, 6, 1, 7, 3, 0},
        {0, 4, 1, 1, 2, 3, 3, 5, 4, 7, 5, 2, 6, 0, 7, 6, 0},
        {0, 4, 1, 1, 2, 3, 3, 6, 4, 2, 5, 7, 6, 5, 7, 0, 0},
        {0, 4, 1, 1, 2, 5, 3, 0, 4, 6, 5, 3, 6, 7, 7, 2, 0},
        {0, 4, 1, 1, 2, 7, 3, 0, 4, 3, 5, 6, 6, 2, 7, 5, 0},
        {0, 4, 1, 2, 2, 0, 3, 5, 4, 7, 5, 1, 6, 3, 7, 6, 0},
        {0, 4, 1, 2, 2, 0, 3, 6, 4, 1, 5, 7, 6, 5, 7, 3, 0},
        {0, 4, 1, 2, 2, 7, 3, 3, 4, 6, 5, 0, 6, 5, 7, 1, 0},
        {0, 4, 1, 6, 2, 0, 3, 2, 4, 7, 5, 5, 6, 3, 7, 1, 0},
        {0, 4, 1, 6, 2, 0, 3, 3, 4, 1, 5, 7, 6, 5, 7, 2, 0},
        {0, 4, 1, 6, 2, 1, 3, 3, 4, 7, 5, 0, 6, 2, 7, 5, 0},
        {0, 4, 1, 6, 2, 1, 3, 5, 4, 2, 5, 0, 6, 3, 7, 7, 0},
        {0, 4, 1, 6, 2, 1, 3, 5, 4, 2, 5, 0, 6, 7, 7, 3, 0},
        {0, 4, 1, 6, 2, 3, 3, 0, 4, 2, 5, 7, 6, 5, 7, 1, 0},
        {0, 4, 1, 7, 2, 3, 3, 0, 4, 2, 5, 5, 6, 1, 7, 6, 0},
        {0, 4, 1, 7, 2, 3, 3, 0, 4, 6, 5, 1, 6, 5, 7, 2, 0},
        {0, 5, 1, 0, 2, 4, 3, 1, 4, 7, 5, 2, 6, 6, 7, 3, 0},
        {0, 5, 1, 1, 2, 6, 3, 0, 4, 2, 5, 4, 6, 7, 7, 3, 0},
        {0, 5, 1, 1, 2, 6, 3, 0, 4, 3, 5, 7, 6, 4, 7, 2, 0},
        {0, 5, 1, 2, 2, 0, 3, 6, 4, 4, 5, 7, 6, 1, 7, 3, 0},
        {0, 5, 1, 2, 2, 0, 3, 7, 4, 3, 5, 1, 6, 6, 7, 4, 0},
        {0, 5, 1, 2, 2, 0, 3, 7, 4, 4, 5, 1, 6, 3, 7, 6, 0},
        {0, 5, 1, 2, 2, 4, 3, 6, 4, 0, 5, 3, 6, 1, 7, 7, 0},
        {0, 5, 1, 2, 2, 4, 3, 7, 4, 0, 5, 3, 6, 1, 7, 6, 0},
        {0, 5, 1, 2, 2, 6, 3, 1, 4, 3, 5, 7, 6, 0, 7, 4, 0},
        {0, 5, 1, 2, 2, 6, 3, 1, 4, 7, 5, 4, 6, 0, 7, 3, 0},
        {0, 5, 1, 2, 2, 6, 3, 3, 4, 0, 5, 7, 6, 1, 7, 4, 0},
        {0, 5, 1, 3, 2, 0, 3, 4, 4, 7, 5, 1, 6, 6, 7, 2, 0},
        {0, 5, 1, 3, 2, 1, 3, 7, 4, 4, 5, 6, 6, 0, 7, 2, 0},
        {0, 5, 1, 3, 2, 6, 3, 0, 4, 2, 5, 4, 6, 1, 7, 7, 0},
        {0, 5, 1, 3, 2, 6, 3, 0, 4, 7, 5, 1, 6, 4, 7, 2, 0},
        {0, 5, 1, 7, 2, 1, 3, 3, 4, 0, 5, 6, 6, 4, 7, 2, 0},
        {0, 6, 1, 0, 2, 2, 3, 7, 4, 5, 5, 3, 6, 1, 7, 4, 0},
        {0, 6, 1, 1, 2, 3, 3, 0, 4, 7, 5, 4, 6, 2, 7, 5, 0},
        {0, 6, 1, 1, 2, 5, 3, 2, 4, 0, 5, 3, 6, 7, 7, 4, 0},
        {0, 6, 1, 2, 2, 0, 3, 5, 4, 7, 5, 4, 6, 1, 7, 3, 0},
        {0, 6, 1, 2, 2, 7, 3, 1, 4, 4, 5, 0, 6, 5, 7, 3, 0},
        {0, 6, 1, 3, 2, 1, 3, 4, 4, 7, 5, 0, 6, 2, 7, 5, 0},
        {0, 6, 1, 3, 2, 1, 3, 7, 4, 5, 5, 0, 6, 2, 7, 4, 0},
        {0, 6, 1, 4, 2, 2, 3, 0, 4, 5, 5, 7, 6, 1, 7, 3, 0},
        {0, 7, 1, 1, 2, 3, 3, 0, 4, 6, 5, 4, 6, 2, 7, 5, 0},
        {0, 7, 1, 1, 2, 4, 3, 2, 4, 0, 5, 6, 6, 3, 7, 5, 0},
        {0, 7, 1, 2, 2, 0, 3, 5, 4, 1, 5, 4, 6, 6, 7, 3, 0},
        {0, 7, 1, 3, 2, 0, 3, 2, 4, 5, 5, 1, 6, 6, 7, 4, 0}};

    int k = 0;
    scanf("%d", &k);
    while (k--)
    {
        unsigned char chess[8][8] = {0};
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                scanf("%hhu", &(chess[i][j]));
        int max = 0;
        for (int i = 0; i < 92; i++)
        {
            int t = 0;
            for (int j = 0; j < 8; j++)
                t += chess[solutions[i][j * 2]][solutions[i][j * 2 + 1]];
            if (t > max)
                max = t;
        }
        printf("%5d\n", max);
    }

    return 0;
}