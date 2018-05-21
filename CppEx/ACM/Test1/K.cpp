/**
* K
*/
 
#include <bits/stdc++.h>  
  
using namespace std;  
  
const int M=10005;  
int x[M],y[105][105];  
void A(int a,int b,int c,int d,int e,int n,int m,int x[],int y[][105])  
{  
    int i;  
    if(e<n*m)  
    {  
        for(i=a; i<=b; ++i)  
            y[c][i]=x[e++];  
        for(i=c+1; i<d&&e<n*m; ++i)  
            y[i][b]=x[e++];  
        for(i=b; i>=a&&e<n*m; --i)  
            y[d][i]=x[e++];  
        for(i=d-1; i>c&&e<n*m; --i)  
            y[i][a]=x[e++];  
        A(a+1,b-1,c+1,d-1,e,n,m,x,y);  
    }  
}  
int main()  
{  
    int n,m,i,j;  
    while(cin>>n>>m)  
    {  
        for(i=0; i<n*m; ++i)  
            cin>>x[i];  
        A(0,m-1,0,n-1,0,n,m,x,y);  
        for(i=0;i<n;++i)  
        {  
            cout<<y[i][0];  
            for(j=1;j<m;++j)  
                cout<<" "<<y[i][j];  
            cout<<endl;  
        }  
    }  
    return 0;  
}  
