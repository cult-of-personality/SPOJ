/* Accepted. Time = 7.04s. Need to improve.*/
#include <cstdio>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

struct element
{
    string str;
    int mod;
};

string bfs(int n)
{
    vector<bool> v(n,false);
    queue<element> q;
    element start,a,b;
    int x,y;
    start.str = '1';
    start.mod = 1;
    q.push(start);
    while(!q.empty()){
        a = q.front();
        x = (a.mod*10)%n;
        y = (a.mod*10 + 1)%n;
        if(x<0)
            x+=n;
        if(y<0)
            y+=n;
        if(x==0)
            return (a.str + '0');
        if(y==0)
            return (a.str + '1');
        if(!v[x]){
            v[x] = true;
            b.str = a.str + '0';
            b.mod = x;
            q.push(b);
        }
        if(!v[y]){
            v[y] = true;
            b.str = a.str + '1';
            b.mod = y;
            q.push(b);
        }
        q.pop();
    }
}

int main(void)
{
    int t,n;
    scanf("%d",&t);
    string s;
    while(t--)
    {
        scanf("%d",&n);
        s = bfs(n);
        cout<<s<<"\n";
    }
    return 0;
}
