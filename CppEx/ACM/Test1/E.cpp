/**
* E
*/

#include <iostream>  
#include <cstring>  
  
using namespace std;  
  
int main()  
{  
    char x[55];  
    int n,a,b,i,j;  
    while(cin>>x)  
    {  
        n=strlen(x);  
        b=0;  
        for(i=0; i<n; ++i)  
        {  
            a=1;  
            for(j=1; i-j>=0&&i+j<n&&x[i-j]==x[i+j]; ++j)  
                a+=2;  
            b=a>b?a:b;  
            a=0;  
            if(x[i]==x[i+1])  
                a=2;  
            for(j=1; x[i]==x[i+1]&&i-j>=0&&i+j+1<n&&x[i-j]==x[i+j+1]; ++j)  
                a+=2;  
            b=a>b?a:b;  
        }  
        cout<<b<<endl;  
    }  
    return 0;  
}  

