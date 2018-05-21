/**
* O
*/

#include<bits/stdc++.h>  
#include<iostream>  
using namespace std;  
int main()  
{  
    int n;  
    int a[20001];  
    while(scanf("%d",&n)!=EOF)  
    {  
        for(int i=0;i<2*n;i++)  
        {  
            scanf("%d",&a[i]);  
            a[i]=fabs(a[i]);  
        }  
        sort(a,a+2*n);  
        double sum=0;  
        for(int i=1;i<=2*n-1;i=i+2)  
        {  
            sum=sum+a[i]-a[i-1];  
        }  
        printf("%.0lf\n",sum);  
    }  
    return 0;  
}  