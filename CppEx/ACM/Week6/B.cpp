/**
 * B: 8 Queens Chess Problem
 * scheme sketch:
 *      brute-force backtrack
 */

#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

int main(void)
{
    vector<string> arr = {"hello", "world"};
    // solutions generated by lay() in A.cpp
    static string solutions[92] = {
        "04752613",
        "05726314",
        "06357142",
        "06471352",
        "13572064",
        "14602753",
        "14630752",
        "15063724",
        "15720364",
        "16257403",
        "16470352",
        "17502463",
        "20647135",
        "24170635",
        "24175360",
        "24603175",
        "24730615",
        "25147063",
        "25160374",
        "25164073",
        "25307461",
        "25317460",
        "25703641",
        "25704613",
        "25713064",
        "26174035",
        "26175304",
        "27360514",
        "30471625",
        "30475261",
        "31475026",
        "31625704",
        "31625740",
        "31640752",
        "31746025",
        "31750246",
        "35041726",
        "35716024",
        "35720641",
        "36074152",
        "36271405",
        "36415027",
        "36420571",
        "37025164",
        "37046152",
        "37420615",
        "40357162",
        "40731625",
        "40752613",
        "41357206",
        "41362750",
        "41506372",
        "41703625",
        "42057136",
        "42061753",
        "42736051",
        "46027531",
        "46031752",
        "46137025",
        "46152037",
        "46152073",
        "46302751",
        "47302516",
        "47306152",
        "50417263",
        "51602473",
        "51603742",
        "52064713",
        "52073164",
        "52074136",
        "52460317",
        "52470316",
        "52613704",
        "52617403",
        "52630714",
        "53047162",
        "53174602",
        "53602417",
        "53607142",
        "57130642",
        "60275314",
        "61307425",
        "61520374",
        "62057413",
        "62714053",
        "63147025",
        "63175024",
        "64205713",
        "71306425",
        "71420635",
        "72051463",
        "73025164"};

    int n = 0;
    cin >> n;
    bool fuckNewline = false;
    while (n--)
    {
        int row, col;
        cin >> row >> col;
        // fuck format
        // printf("SOLN       COLUMN\n #      1 2 3 4 5 6 7 8\n\n");
        // fuck new line
        if (fuckNewline)
            printf("\n");
        else
            fuckNewline = true;
        printf("SOLN       COLUMN\n");
        printf(" #      1 2 3 4 5 6 7 8\n\n");
        int count = 0;
        for (int i = 0; i < 92; i++)
        {
            if (solutions[i][col - 1] == row - 1 + '0')
            {
                // output
                printf("%2d      ", ++count);
                for (int j = 0; j < 7; j++)
                    printf("%d ", ((int) (solutions[i][j]) - '0') + 1);
                // end of line not contain space....
                printf("%d\n", ((int) (solutions[i][7] - '0')) + 1);
            }
        }
        // printf("\n");
    }

    return 0;
}
