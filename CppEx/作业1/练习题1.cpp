#include <iostream>

using namespace std;

int main()
{
    int year,month,day;
    int all;
    cout<<"请输入年月日"<<endl;
    cin>>year>>month>>day;
    if(year%400==0){all=366;}
    else
    {
        if(year%4==0&&year*100!=0){all=366;}
        else {all=365;}
    }
    if(month==1||month==3||month==5||month==7||month==8||month==10||month==12) {day=31;}
    else
    {
        if(month==4||month==6||month==9||month==11) {day=30;}
        else if(month==2)
        {
        if(all==365) day=28;
        if(all==366) day=29;
        }
    }
    cout<<"该月天数为："<<endl;
    cout<<day;
}
