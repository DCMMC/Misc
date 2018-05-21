/**
* C
*/

#include<iostream>  
#include<stdio.h>  
#include<string.h>  
#include<algorithm>  
#include<map>  

using namespace std;  
  
long long a[25];  
map<long long,int>m;  
long long ans;  
int n;  
void dfs(int sum,int i)  
{  
    if(i == n+1)  
    {  
       if(m[sum]==0) ///没有出现过  
       {  
           m[sum] = 1;  
           ans++;  
       }  
       return;  
    }  
    dfs(sum+a[i],i+1);  
    dfs(sum-a[i],i+1);  
}  
int main()  
{  
    while(~scanf("%d",&n))  
    {  
        for(int i = 1; i <= n; i++)  
            scanf("%d",&a[i]);  
        m.clear();  
        ans = 0;  
        dfs(0,1);  
        printf("%d\n",ans);  
    }  
    return 0;  
}  
