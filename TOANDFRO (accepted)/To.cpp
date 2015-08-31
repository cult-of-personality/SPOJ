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
#define SORT(x) sort(x.begin(),x.end())

string decrypt(string s,int n){
    if(s.length()<=n)
        return s;
    vector<string> v;
    int i=0;
    bool flag = false;
    while(i<s.length()){
        string t = s.substr(i,n);
        if(flag)
            reverse(t.begin(),t.end());
        v.push_back(t);
        flag = !flag;
        i += n;
    }
    string r;
    FOR(i,0,n){
        FOR(j,0,v.size()){
            if(v[j].length()>i)
                r += v[j][i];
        }
    }
    return r;
}

int main(void){
    FASTIO
    int n;
    string s;
    while(1){
        cin>>n;
        if(n==0)
            break;
        cin>>s;
        cout<<decrypt(s,n)<<"\n";
    }
}
