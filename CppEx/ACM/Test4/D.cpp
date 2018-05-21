/**
 * D: Robot
 * 
 * ! 组合排列 + 阶乘问题
 * ! 这是一个放回(也就是可重复)的取球实验, 从 n 个球要放回的取 m 次不同的排列个数, 可以先逆向思维,
 * ! 也就是 m 个一模一样的球放在 n 个格子中, 也就是有 n - 1 隔板和 m 个球的组合排列问题, 先将 m 个球和
 * ! n - 1个隔板全排列: (m + n - 1)!
 * ! 然后因为 m 个球一模一样, n - 1块隔板也一模一样, 所以最后的答案就是 (m + n - 1)! / (m! * (n - 1)!)
 * 
 * ! 此题比想象的还难, 是一个组合数学和数论题
 * 
 * ! 按照题目要求, 这个跟卡特兰(Catalan)数的定义一模一样
 * ! 在一维轴上, 如果只能向两个方向走也不能停顿, 那么 2n 步后还是在原点可以产生的不同序列有 \frac{(2n)!}{n! * n!}
 * ! 而其中到过原点左侧的序列有 \frac{(2n)!}{(n - 1)! * (n + 1)!} 种, 因为其路径按照 Ref 中的 图, 等价于
 * ! 用 x = 1 点通过 2n 次步数达到 x = -1 点的序列数目
 * 
 * ! 所以 Catalan(n) = \frac{C_{2n}^n}{n + 1} = \frac{4n - 2}{n + 1} Catalan(n - 1)
 * ! 用这个递推公式就可以打表了, 然而题目还要一个带模除法 (mod 100000007)
 * ! 这个带模除法又是一个难点, 需要用到费马小定理(或者扩展欧几里得算法):
 * ! 如果 M 是一个素数且 b 不是 M 的倍数, 则有 b ^ ( M - 1) \equiv 1 (mod M)
 * ! 所以 a / b \equiv a / b * (b * b ^ (M - 2)) \equiv a * b^(M - 2) (mod M)
 * ! 所以 b 关于 M 的逆元就是 inv(b) = b^(M - 2)
 * 
 * ! 还有就是数论基本运算定理:
 * ! (a * b) % c = ((a % c) * (b % c)) % c 而除法没有这个性质, 所以必须使用逆元(也就是用费马小定理)
 * 
 * ! 当 M 是质数的时候, 还有以下性质: inv(b) = (M - M / b) * inv(M % b) % M
 * ! 证明:
 * ! 令 x = M % b, y = M / b
 * ! 显然 x + y * b = M
 * ! (x + y * b) % M = 0
 * ! x % M = (-y * b) % M
 * ! (x * inv(b)) % M = (-y) % M
 * ! (x * inv(b)) % M = (M - y) % M
 * ! inv(b) = inv(x) * (M - y) % M
 * ! = inv(M % b) * (M - M / b) % M
 * ! 利用以上性质就可以实现 O(n) 递推打表
 * 
 * ! Ref: http://www.mtholyoke.edu/~jjlee/Teaching/Intro_to_Catalan.pdf
 */

#include <cstdio>

#define ull_t unsigned long long

const ull_t MOD = (1e9 + 7);
const ull_t MAXN  = (1e6 + 7);


// 逆元
ull_t inv[MAXN + 2];
// Catalan数
ull_t cat[MAXN + 1];
// 阶乘表
ull_t fact[MAXN + 1];
// 组合数逆元打表
ull_t comb_inv[MAXN + 1];

// 快速幂运算
// a^b mod c
// 也就是把幂进行 log(N) 的变换
ull_t powerMode(ull_t a, ull_t b, ull_t c)
{
    ull_t ans = 1, base = a;
    while (b)
    {
        if (b & 1)
        {
            // 如果现在的 b 是偶数
            // 取模除数可以有分配律
            ans = (ans * base) % c;
        }
        base = (base * base) % c;
        // 右移一位
        b >>= 1;
    }

    return ans;
}

void init()
{
    // 递推逆元打表
    inv[1] = 1;
    inv[0] = 1;
    for (ull_t i = 2; i <= MAXN + 1; i++)
    {
        inv[i] = inv[MOD % i] * (MOD - MOD / i) % MOD;
    }
    // 用逆元和递推公式递推打表卡特兰数(因为题目要求都要结果取模)
    cat[1] = 1;
    cat[0] = 1;
    for (ull_t i = 2; i <= MAXN; i++)
    {
        cat[i] = ((cat[i - 1] * (4 * i - 2)) % MOD) * inv[i + 1] % MOD;
    }
    // 阶乘打表
    fact[0] = 1;
    fact[1] = 1;
    for (ull_t i = 2; i <= MAXN; i++)
    {
        fact[i] = (fact[i - 1] * i) % MOD;
    }
    // 组合数逆元打表
    comb_inv[1] = 1;
    comb_inv[0] = 1;
    for (ull_t i = 2; i <= MAXN; i++)
    {
        comb_inv[i] = comb_inv[MOD % i] * (MOD - MOD / i) % MOD;
    }
}

int main(int argc, char const *argv[])
{
    // [1, 100]
    int T = 0;
    init();
    scanf("%d", &T);
    while (T--)
    {
        // [1, 1,000,000]
        long n = 0;
        scanf("%ld", &n);
        // 只能往右走或者步骤或者往左走(不在原点才能往左)
        ull_t ans = 0ULL;
        for (long i = 0; i <= n / 2; i++)
        {
            // 这里的 i 是走出去的步数的一半
            // (n!) / ((n - 2 *i)! * (2*i)!) mod MOD * cat[i]
            ans += (((((fact[n]*comb_inv[fact[2*i]]%MOD)*comb_inv[fact[n-2*i]])%MOD)*cat[i])%MOD);
        }
        
        printf("%llu\n", ans);
    }

    return 0;
}
