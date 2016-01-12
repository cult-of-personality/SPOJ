//accepted \m/
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
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
#define SI short int
#define pb push_back
#define mp make_pair
#define pii pair<int,int>
#define pbc pair<bool,char>
#define pcc pair<char,char>
#define vi vector<int>
#define vii vector<vector<int> >
#define vb vector<bool>
#define FOR(i,st,end) for(int (i)=(st);i<(end);i++)
#define FORD(i,st,end) for(int (i)=(st);i>=(end);i--)
#define FASTIO ios::sync_with_stdio(false);
#define ABS(i) ((i)>0)?(i):(-(i))
#define sci(m) scanf("%d",&m)
#define SORT(x) sort(x.begin(),x.end())
#define MOD 1000000007
#define INT_MIN (-2147483647 - 1)

int gcd(int a,int b){
    int t;
    while(b!=0){
        t = a%b;
        a = b;
        b = t;
    }
    return a;
}

bool isPossible(int a,int b,int c){
    int d = gcd(a,b);
    if(c%d != 0)
        return false;
    if(c>max(a,b))
        return false;
    return true;
}

int steps(int a,int b,int c){
    map<pii,int> m;
    queue<pii > q;
    pii p,r,s;
    int k,cap;
    m[pii(0,0)] = 0;
    q.push(pii(0,0));
    while(!q.empty()){
        p = q.front();
        k = m[p];
        q.pop();
        if(p.first==c || p.second==c)   //reached target.
            return k;
        r = pii(0,p.second);
        if(m.find(r)==m.end()){   //emptying first vessel.
            m[r] = k+1;
            q.push(r);
        }
        r = pii(p.first,0);
        if(m.find(r)==m.end()){ //emptying second vessel.
            m[r] = k+1;
            q.push(r);
        }
        r = pii(a,p.second);
        if(p.first==0 && m.find(r)==m.end()){  //filling first vessel
            m[r] = k+1;
            q.push(r);
        }
        r = pii(p.first,b);
        if(p.second==0 && m.find(r)==m.end()){  //filling second vessel
            m[r] = k+1;
            q.push(r);
        }
        cap = min(a-p.first,p.second);      //second vessel into first
        r = pii(p.first+cap,p.second-cap);
        if(m.find(r)==m.end()){
            m[r] = k+1;
            q.push(r);
        }
        cap = min(p.first,b-p.second);  //first vessel into second.
        r = pii(p.first-cap,p.second+cap);
        if(m.find(r)==m.end()){
            m[r] = k+1;
            q.push(r);
        }
    }
    return 0;
}

int main(void){
    int t,a,b,c;
    sci(t);
    while(t--){
        scanf(" %d %d %d",&a,&b,&c);
        if(!isPossible(a,b,c))
            printf("-1\n");
        else printf("%d\n",steps(a,b,c));
    }
    return 0;
}
