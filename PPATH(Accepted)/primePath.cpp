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

vb f(10001,false);
map<int,int> m;
vii g;
vi primes;

string toString(int n){
    string s;
    if(n==0)
        return "0";
    while(n){
        s += (char)('0' + n%10);
        n/=10;
    }
    reverse(s.begin(),s.end());
    return s;
}

int toNum(string s){
    int n = 0;
    FOR(i,0,s.length()){
        n = 10*n + (int)(s[i]-'0');
    }
    return n;
}

void getPrimes(){
    //vi primes;
    FOR(i,2,f.size()){
        if(f[i])
            continue;
        if(i>1000)
            primes.pb(i);
        for(int j=i*i;j<f.size();j+=i)
            f[j] = true;
    }
}

void buildMap(){
    FOR(i,0,primes.size())
        m[primes[i]] = i;
}

void buildGraph(){
    FOR(i,0,primes.size()){
        string s = toString(primes[i]);
        string t;
        FOR(k,0,s.length()){
            t = s;
            for(char c = s[k];c<='9';c++){
                t[k] = c;
                int n = toNum(t);
                if(n>1000 && n<10000 && f[n]==false){
                    //n is a prime.
                    if(n>primes[i]){
                        g[i].pb(n);
                        g[m[n]].pb(primes[i]);
                    }
                }
            }
        }
    }
}

int main(void){
    int t,a,b;
    sci(t);
    getPrimes();
    buildMap();
    g.resize(primes.size());
    buildGraph();
    while(t--){
        scanf("%d %d",&a,&b);
        vi done(primes.size(),MOD);
        queue<int> q;
        q.push(a);
        done[m[a]] = 0;
        while(!q.empty()){
            int n = q.front();
            q.pop();
            //printf("N = %d\n",n);
            if(n==b)
                break;
            int idx = m[n];
            FOR(i,0,g[idx].size()){
                //printf("%d ",g[idx][i]);
                if(done[idx]+1 < done[m[g[idx][i]]]){
                    done[m[g[idx][i]]] = done[idx]+1;
                    q.push(g[idx][i]);
                }
            }
            //printf("\n");
        }
        if(done[m[b]]==MOD)
            printf("Impossible\n");
        else printf("%d\n",done[m[b]]);
    }
    return 0;
}
