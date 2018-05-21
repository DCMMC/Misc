/**
* I
*/

#include<stdio.h>  
#include<string.h>  
#include<algorithm>  
#include<iostream>  
using namespace std;  
int a[1000005];  
int main()  
{  
    int n;  
    while(~scanf("%d",&n))  
    {  
        int i,j=0,m=n-1;//  
        memset(a,0,sizeof(a));  
        for(i=2;i<n;i++)  
        {  
            if(n%i==0)//满足是约数  
            {  
                for(j=1;j*i<n;j++)  
                if(a[j*i]==0)//约数的倍数  
                {  
                    a[j*i]=1;//因为约数的倍数可能会有重复，所以用到标记数组  
                    m--;  
                }  
            }  
        }  
        printf("%d\n",m);  
    }  
    return 0;  
}  