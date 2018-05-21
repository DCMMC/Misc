/**
 * Greedy Gift Givers
 * 
 * 就是一个字符串查找的(或者说是用 map 的)
 * 
 * ! 一定要记得除法的时候检查下分母有不有可能取到 0
 * ! 题目说的是对于钱分出去除不整的时候尽可能多的取到整数, 所以所花费的钱要跟实际输出一样多, 不一定就是
 * ! 输入的那个数值
 * ! map 的时候不管是添加还是获取都可以直接用 operator[]
 */
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
    int N;
    bool f = 1;
    while (cin >> N)
    {
        if (f)
        {
            f = 0;
        }
        else
        {
            cout << "\n";
        }

        map<string, int> m;
        string s;
        vector<string> l;
        for (int i = 0 ; i < N; i++)
        {
            cin >> s;
            m[s] = 0;
            l.push_back(s);
        }
        for (int i = 0; i < N; i++)
        {
            cin >> s;
            int sum;
            int person;
            cin >> sum >> person;
            m[s] += person > 0 ? - ((sum / person) * person) : 0;
            for (int j = 0; j < person; j++)
            {
                cin >> s;
                m[s] += sum / person;
            }
        }

        for (int i = 0; i < N; i++)
        {
            cout << l[i] << " " << m[l[i]] << "\n";
        }
    }

    return 0;
}
