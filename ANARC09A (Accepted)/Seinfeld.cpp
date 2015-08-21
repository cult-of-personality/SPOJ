//replace operation allowed on braces.
//accepted.
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

int main(void){
    FASTIO
    int t=0;
    string s;
    while(1){
        cin>>s;
        if(s[0]=='-')
            break;
        t++;
        stack<int> st;
        int ans = 0;
        FOR(i,0,s.length()){
            if(s[i]=='{')
                st.push(i);
            else{
                if(st.empty()){
                    ans++;
                    st.push(i);
                }
                else{
                    st.pop();
                }
            }
        }
        while(!st.empty()){
            //
            st.pop();
            ans++;
            if(!st.empty())
                st.pop();
        }
        cout<<t<<". "<<ans<<"\n";
    }
    return 0;
}
