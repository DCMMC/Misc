#include <iostream>

using namespace std;

void expand(char *s,char *t)
{

    for(int i=0,j=0;s[i]!='\0';i++){

        switch(s[i])
        {
            case '\n':t[j++]='\\';
                    t[j++]='n';
                    break;
            case '\t':t[j++]='\\';
                    t[j++]='t';
                    break;
            default:t[j++]=s[i];
                break;
        }
    t[j]=0;

    }
}

int main()
{
    char s[10]={'a','\t','b'};
    char t[10];
    expand(s,t);
    cout<<t;
}
