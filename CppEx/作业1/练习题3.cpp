#include <iostream>

using namespace std;

int compare(int &x,int &y,int &z)//������Ϊ int & return�޷��ı� 
{
   int temp;
   if(x>=y)
   {
       temp = x;
       x = y;
       y = temp;
   }
   if(y>=z)
   {
   		temp=z;
   		z=y;
   		y=temp;
   }
   if(x>z)
     {temp=z;z=x;x=temp;};
    
    
}

int main()
{
    int a,b,c;
    int max;
    cout<<"������������"<<endl;
    cin>>a>>b>>c;
    
    cout<<"��С��������"<<endl;
    compare(a,b,c);
    cout<<a<<" "<<b<<" "<<c;
    
    return 0;
    
}
