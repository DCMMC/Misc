#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    float x[8]={0};
    float aver(0),max=0,min=100;
    for(int i=0;i<8;i++)
    {
        cin>>x[i];
        if(x[i]>max)
            max=x[i];
        if(x[i]<min)
            min=x[i];
        aver+=x[i];
        cout<<x[i]<<endl;
    }
    aver=(aver-min-max)/6.00;
    cout<<fixed<<setprecision(2)<<aver<<endl;
    return 0;
}
