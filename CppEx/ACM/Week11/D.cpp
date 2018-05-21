/**
 * Word Scramble
 * 
 * easy
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[])
{
    string line;
    // 下次还是循环用 getchar
    while (getline(cin, line)) {
        stringstream iss(line);
        vector<string> v;
        string w;
        iss >> w;
        reverse(w.begin(), w.end());
        cout << w;
        while (iss >> w) {
            reverse(w.begin(), w.end());
            cout << " " << w;
        }
        cout << "\n";
    }

    return 0;
}
