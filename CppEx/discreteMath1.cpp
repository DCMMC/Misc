/**
* Discrete Math Ex2
* Set operations
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string> // getline
#include <sstream> // iss

using namespace std;

int main(void) {
    vector<int> A, B, C, D;
    string line;
    int tmp;

    cout << "A(上课的学生): \n";
    getline(cin, line);
    istringstream iss(line);
    while (iss >> tmp)
        A.push_back(tmp);
    
    cout << "B(男生): \n";
    getline(cin, line);
    iss = istringstream(line);
    while (iss >> tmp)
        B.push_back(tmp);
    
    cout << "C(女生):\n";
    getline(cin, line);
    iss = istringstream(line);
    while (iss >> tmp)
        C.push_back(tmp);
    
    cout << "D(考试合格):\n";
    getline(cin, line);
    iss = istringstream(line);
    while (iss >> tmp)
        D.push_back(tmp);

    sort(A.begin(), A.end());
    sort(B.begin(), B.end());
    sort(C.begin(), C.end());
    sort(D.begin(), D.end());

    cout << "没来上课的同学:\n";
    vector<int> result, result2;
    // B + C
    set_union(B.begin(), B.end(), C.begin(), C.end(), inserter(result, result.begin()));
    // B + C - A
    set_difference(result.begin(), result.end(), A.begin(), A.end(), inserter(result2, result2.begin()));
    for (auto &i : result2)
        cout << i << " ";
    cout << endl;

    cout << "考试不及格的同学:\n";
    // B + C - D
    vector<int> result3;
    set_difference(result.begin(), result.end(), D.begin(), D.end(), inserter(result3, result3.begin()));
    for (auto &i : result3)
        cout << i << " ";
    cout << endl;

    cout << "没来上课且考试不及格的男生\n";
    // B^(B + C - A)^(B + C - D)
    vector<int> result4, result5;
    set_intersection(result2.begin(), result2.end(), result3.begin(), result3.end(), 
        back_inserter(result4));
    set_intersection(B.begin(), B.end(), result4.begin(), result4.end(), 
        back_inserter(result5));
    for (auto &i : result5)
        cout << i << " ";
    cout << endl;
    

    return 0;
}