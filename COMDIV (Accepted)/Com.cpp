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
#define FOR(i,st,end) for(int (i)=(st);i<(end);i++)
#define FORD(i,st,end) for(int (i)=(st);i>=(end);i--)
#define FASTIO ios::sync_with_stdio(false);
#define ABS(i) ((i)>0)?(i):(-(i))
#define SORT(x) sort(x.begin(),x.end())

int gcd(int a,int b){
    int t;
    while(b!=0){
        t = a%b;
        a = b;
        b = t;
    }
    return a;
}

void sieve(vector<int> &p){
    vector<bool> s(1001,false);
    FOR(i,2,s.size()){
        if(s[i])
            continue;
        p.push_back(i);
        for(int j=i*i;j<s.size();j+=i){
            s[j] = true;
        }
    }
}

int main(void){
    FASTIO
    int t,a,b,g;
    scanf("%d",&t);
    vector<int> primes;
    sieve(primes);
    while(t--){
        scanf("%d %d",&a,&b);
        g = gcd(a,b);
        if(g==1)
            printf("1\n");
        else{
            int ans = 1,p=0,c;
            while(g>=0 && p<primes.size()){
                c = 0;
                while(g%primes[p] == 0){
                    c++;
                    g /= primes[p];
                }
                ans *= (c+1);
                p++;
            }
            if(g>1)
                ans *= 2;
            printf("%d\n",ans);
        }
    }
}
