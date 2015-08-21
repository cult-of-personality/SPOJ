//Done recursively... Why memoize??
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

int f(string &s,int start,int low){ //recursively.
    if(start>=s.length())
        return 1;
    int j = start+1;
    int sum = (int)(s[start]-'0');
    while(j<s.length() && sum<low){
        sum += (int)(s[j]-'0');
        j++;
    }
    if(j==s.length() && sum<low)    //even the whole string ain't enough;
        return 0;
    int ans = 0;
    while(j<=s.length()){
        ans += f(s,j,sum);
        if(j<s.length())
            sum += (int)(s[j]-'0');
        j++;
    }
    return ans;
}

int main(void){
    int t=0;
    string s;
    FASTIO
    cin>>s;
    while(s!="bye"){
        t++;
        int ans = f(s,0,0);
        cout<<t<<". "<<ans<<"\n";
        cin>>s;
    }
    return 0;
}
