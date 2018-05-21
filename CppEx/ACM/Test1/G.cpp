/**
* G
*/

#include<iostream>  
#include<stdio.h>  
#include<string.h>  
#include<algorithm>  
#include<queue>  
using namespace std;  
  
int main()  
{  
    long long m,n;  ///m行n列  
    while(~scanf("%lld%lld",&m,&n))  
    {  
        long long step = (m-1)+(n-1);  
        if(step%2==0)  ///偶数步，不用堵就可以赢  
            printf("YES\n");  
        else  
        {  
            int a,b;  
            if(m<n)    
            {  
                a = m; b = n;  
            }  
            else  
            {  
                a = n; b = m;  
            }   
            ///上面都啰嗦，其实看abs(m-n)就OK  
            if(b-a == 1)  
                printf("NO\n");  
            else  
                printf("YES\n");  
        }  
    }  
    return 0;  
}  
