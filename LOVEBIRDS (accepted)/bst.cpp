#include <vector>
#include <iostream>
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
#define SORT(x) sort(x.begin(),x.end())
#define MOD 1908

void buildDP(vector<int> &dp){
    dp[0] = dp[1] = 1;
    FOR(i,2,dp.size()){
        dp[i] = 0;
        FOR(j,0,i){
            dp[i] = (dp[i] + (dp[j]*dp[i-j-1])%MOD)%MOD;
        }
    }
}

int main(void){
    vector<int> dp(1001,-1);
    dp[1] = 1;
    int n,t;
    buildDP(dp);
    cin>>t;
    while(t--){
        cin>>n;
        cout<<dp[n]<<"\n";
    }
}
