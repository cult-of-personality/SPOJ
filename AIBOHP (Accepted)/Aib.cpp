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

int minInsert(string &s){
    if(s.length()==0)
        return 0;
    vector<vector<int> > dp(s.length(),vector<int>(s.length(),0));
    FOR(i,0,s.length()-1){
        if(s[i]!=s[i+1])
            dp[i][i+1] = 1;
    };
    int start,end;
    FOR(k,3,s.length()+1){
        FOR(i,0,s.length()-k+1){
            start = i;
            end = i+k-1;
            if(s[start]==s[end])
                dp[start][end] = dp[start+1][end-1];
            else
                dp[start][end] = min(dp[start][end-1],dp[start+1][end])+1;
        }
    }
    return dp[0][s.length()-1];
}

int main(void){
    string s;
    int t;
    FASTIO
    cin>>t;
    while(t--){
        cin>>s;
        cout<<minInsert(s)<<"\n";
    }
    return 0;
}
