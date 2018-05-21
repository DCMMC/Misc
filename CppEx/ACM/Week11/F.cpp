/**
 * Kindergarten Counting Game
 * 
 * 数单词, 循环 字符串处理
 */

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char const *argv[]) 
{
    string line;
    while (getline(cin, line)) {
        int cnt = 0;
        bool f = 0;
        for (char c : line) {
            if (!f && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
            {
                cnt++;
                f = 1;
            } else if (f && !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
                f = 0;
            }
        }

        cout << cnt << endl;
    }
    return 0;
}
