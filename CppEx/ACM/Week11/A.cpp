/**
 * A: Triangle Wave
 * 简单的循环
 */

#include <iostream>

using namespace std;

void wave(int A, int F)
{
    while (--F)
    {
        for (int i = 1; i <= A; i++)
        {
            for (int j = 0; j < i; j++)
                cout << i;
            cout << "\n";
        }
        for (int i = A - 1; i > 0; i--)
        {
            for (int j = 0; j < i; j++)
                cout << i;
            cout << "\n";
        }
        cout << "\n";
    }

    for (int i = 1; i <= A; i++)
    {
        for (int j = 0; j < i; j++)
            cout << i;
        cout << "\n";
    }
    for (int i = A - 1; i > 0; i--)
    {
        for (int j = 0; j < i; j++)
            cout << i;
        cout << "\n";
    }
}

int main(int argc, char const *argv[])
{
    // num of cases
    int n = 0;
    cin >> n;

    // 0 < Amplitude <= 9
    int A = 0;
    // 0 < Frequency
    int F = 0;
    while (--n)
    {
        cin >> A >> F;
        wave(A, F);
        cout << "\n";
    }

    cin >> A >> F;
    wave(A, F);

    return 0;
}
