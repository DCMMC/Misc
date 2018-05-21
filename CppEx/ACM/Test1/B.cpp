/**
* B: Sailing
*/

#include <iostream>
#include <bits/stdc++.h>

using namespace std;
char Map[55][55];
int n,vis[55][55],dir[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
struct Node
{
    int x,y,power;
    bool friend operator <(Node a,Node b)
    {
        return a.power>b.power;
    }
};
int check(int x,int y)
{
    if(x>=0&&x<n&&y>=0&&y<n)
        return 1;
    return 0;
}
void BFS()
{
    memset(vis,0x3f3f3f3f,sizeof(vis));
    priority_queue<Node> Q;
    int flag;
    Node cur,next;
    cur.x = 0;cur.y = 0;
    cur.power = 0;
    vis[0][0] =  1;
    Q.push(cur);
    while(!Q.empty())
    {
        cur = Q.top();Q.pop();
        if(cur.x==n-1&&cur.y==n-1)
        {
            cout<<cur.power<<endl;
            return;
        }
        if(Map[cur.x][cur.y]=='*')
            flag = 1;
        else
            flag = 2;
        for(int i=0;i<4;i++)
        {
            next.x = cur.x+dir[i][0];
            next.y = cur.y+dir[i][1];
            if(check(next.x,next.y))
            {
                if(flag == 1)
                {
                    if(Map[next.x][next.y]=='#')
                        next.power = cur.power +1;
                    else
                        next.power = cur.power;
                }
                else
                    next.power = cur.power +1;
                if(vis[next.x][next.y] > next.power)//判断更新vis数组的值来确定当前点是否需要入队
                {
                    vis[next.x][next.y] = next.power;
                    Q.push(next);
                }

            }
        }
    }
}
int main()
{
    while(~scanf("%d",&n))
    {
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                cin>>Map[i][j];
        BFS();
    }
    return 0;
}