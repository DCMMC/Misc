/**
* M
*/

#include <iostream>  
  
using namespace std;  
  
int main()  
{  
    int n,m,i,j,a,b;  
    long long D[105][105],K[8][8],C[105][105];  
    while(cin>>n>>m)  
    {  
        for(i=0; i<n; ++i)  
            for(j=0; j<m; ++j)  
                cin>>D[i][j];  
        for(i=0; i<3; ++i)  
            for(j=0; j<3; ++j)  
                cin>>K[i][j];  
        for(i=0; i<n; ++i)  
            for(j=0; j<m; ++j)  
            {  
                C[i][j]=0;  
                for(a=0; a<=min(n-i-1,2); ++a)  
                    for(b=0; b<=min(m-j-1,2); ++b)  
                        C[i][j]+=D[i+a][j+b]*K[a][b];  
            }  
        for(i=0; i<n; ++i)  
        {  
            cout<<C[i][0];  
            for(j=1; j<m; ++j)  
                cout<<" "<<C[i][j];  
            cout<<endl;  
        }  
    }  
    return 0;  
}  
