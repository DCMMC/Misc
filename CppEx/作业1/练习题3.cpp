#include <iostream>

using namespace std;

int compare(int &x,int &y,int &z)//不定义为 int & return无法改变 
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
    cout<<"请输入三个数"<<endl;
    cin>>a>>b>>c;
    
    cout<<"由小到大排序"<<endl;
    compare(a,b,c);
    cout<<a<<" "<<b<<" "<<c;
    
    return 0;
    
}
