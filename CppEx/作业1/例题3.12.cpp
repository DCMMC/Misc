#include <iostream>

using namespace std;

int max1(int a[],int n)
{
    int t=0;
    for(int i=0;i<n;i++)
        if(a[i]>a[t]) t=i;
    return a[t]+0;
}

int& max2(int a[],int n)
{
    int t=0;
    for(int i=0;i<n;i++)
        if(a[i]>a[t]) t=i;
    return a[t];
}

int& sum(int a[],int n)
{
    int s=0;
    for(int i=0;i<n;i++)
        s+=a[i];
    return s;
}

int main()
{
    int a[10]={1,2,3,4,5,6,7,8,9,10};
    int m1=max1(a,10);
    int m2=max2(a,10);
    int &m3=max2(a,10);
    int &m4=sum(a,10);
    cout<<"m1="<<m1<<endl;
    cout<<"m2="<<m2<<endl;
    cout<<"m3="<<m3<<endl;
    cout<<"m4="<<m4<<endl;
    m3+=10;
    max2(a,10)-=100;
    cout<<sum(a,10)<<endl;

    return 0;
}
