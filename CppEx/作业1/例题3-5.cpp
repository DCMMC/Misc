#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * non-recursive binary search, O(logN)
 * * @return return -1 if not found
 */
int binarySearch(const vector<string> &names, const string &key) {
    int lo = 0, hi = names.size(), mid = 0;
    while (lo <= hi) {
        mid = (hi + lo) / 2;
        if (names[mid] < key)
            lo = mid + 1;
        else if (names[mid] > key)
            hi = mid - 1;
        else
            return mid;
    }
    return -1;
}

int main(void) {
    vector<string> names = {"GongJing","LiuNa","HuangPin","ZhouZijun",
        "LianXiaolei","ChenHailing","CuiPeng","LiuPing"};
    
    // O(NlogN)
    sort(begin(names), end(names));
    for(int i = 0; i < names.size(); i++)
        cout << i + 1 << '\t' << names[i] << endl;
    cout << "Input the name to search:";
    string name;
    cin >> name;
    int n = -1;
    if( (n = binarySearch(names, name)) > -1)
        cout << "Position:" << (n + 1) << endl;
    else
        cout << "Not found!" << endl;
    
    return 0;
}
