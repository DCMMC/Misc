#include <iostream>

using namespace std;
const int PI=3.14;

const int *Fun(void)
{
    const int a=5;
    const int *p=&a;
    cout<<"Value of local const wariable a :"<<a<<endl;
    cout<<"Address of local const variable a:"<<&a<<endl;
    cout<<"Value of local const pointer p:"<<p<<endl;
    cout<<"Value of local const variable a :"<<*p<<endl<<endl;
    return p;
}

int main()
{
    const int *q;
    q=Fun();
    cout<<"Main():"<<endl;
    cout<<"Value of local const pointer q:"<<q<<endl;
    cout<<"The return balue of the function Func():"<<*q<<endl;
    const char *str="123ABC";
    cout<<"Address of string const varibale:"<<(void *)str<<endl;
    cout<<"Value of string const variablestr;"<<str<<endl;
    cout<<"Address of global const variable PI:"<<&PI<<endl;
    cout<<"Address of the function Fun():"<<(void *)Fun<<endl;
    return 0;
}
