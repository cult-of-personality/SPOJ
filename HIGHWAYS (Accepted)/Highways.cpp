//Dijkstra's Algorithm using priority queue
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>    //also contains priority_queue
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;
#define LL long long int
#define pb push_back
#define mp make_pair
#define pii pair<int,int>
#define psi pair<int,short int>
#define INF (int)(1e9)
#define FOR(i,st,end) for(int (i)=(st);i<(end);i++)
#define FORD(i,st,end) for(int (i)=(st);i>=(end);i--)

struct comp {
    bool operator() (const pii &a, const pii &b) {
       return a.second > b.second;
    }
};

bool compfun(pii a,pii b)
{
    if(a.second!=b.second)
        return (a.second<b.second);
    if(a.first!=b.first)
        return (a.first<b.first);
    return false;
}

int calculateDist(int v,int edges,vector<psi > mat[],vector<bool> &visited,int b,int e,vector<int> &dist)
{
    priority_queue< pii,vector< pii >,comp> q;
    dist[b] = 0;
    q.push(pii(b,0));   //starting point;
    pii n;
    int neighbor,x,y;
    while(!q.empty()){
        n = q.top();
        q.pop();
        if(visited[n.first])
            continue;
        else if(n.first>v)
            return -1;
        else if(n.first==e)
            return n.second;
        x = n.first;
        y = n.second;
        //dist[x] = y;  Is this necessary?
        FOR(i,0,mat[n.first].size()){
            //for all adjacent nodes.
            neighbor = mat[x][i].first;
            if(visited[neighbor]==false && (y+mat[x][i].second)<dist[neighbor]){
                dist[neighbor] = (y+mat[x][i].second);
                q.push(pii(neighbor,dist[neighbor]));
            }
        }
        visited[x] = true;
    }
    return -1;
}

int main(void)
{
    ios_base::sync_with_stdio(0);
    int t,v,e,a,b,res,st,en;
    short int d;
    cin>>t;
    FOR(i,0,t){
        cin>>v>>e>>st>>en;
        vector<bool> visited(v+1,false);
        vector<psi > mat[v+1];
        vector<int> dist(v+1,INF);
        FOR(j,0,e){
            cin>>a>>b>>d;
            mat[a].pb(psi(b,d));
            mat[b].pb(psi(a,d));
        }
        res = calculateDist(v,e,mat,visited,st,en,dist);
        if(res==-1 || dist[en]==INF)
            cout<<"NONE\n";
        else cout<<res<<"\n";
    }
    return 0;    
}
