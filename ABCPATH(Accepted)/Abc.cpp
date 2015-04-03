/*  Accepted.
    Once you visit node, you should know the max length of the chain originating from there.
    If that node comes up in another DFS, then no need to go deep again. */
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

typedef pair<short int,short int> cd;
bool visited[51][50];
char grid[51][50];
short int ans[51][50];
short int h,w,maxlen;
short int xd[] = {-1,-1,-1,0,0,1,1,1};
short int yd[] = {-1,0,1,-1,1,-1,0,1};
vector<cd> start;

inline bool exists(short int a,short int b){
    return ((a>=0)&&(a<h)&&(b>=0)&&(b<w));
}

void DFS(int a,int b)
{
   //rooted at A.
   short int x,y;
   short int curr=0;
    for(int i=0;i<8;i++){
        x = a+xd[i];
        y = b+yd[i];
        if((exists(x,y))&&(grid[x][y]-grid[a][b]==1))
        {
            if(!visited[x][y])
                DFS(x,y);
            if(curr<ans[x][y])
                curr = ans[x][y];            
        }
    }
    ans[a][b] = curr+1;
    visited[a][b] = true;
}

int main(void)
{
    int t=1;
    short int a,b;
    while(true){
        scanf(" %hd %hd",&h,&w);
        if((h==0)&&(w==0))
            break;
        start.clear();
        for(int i=0;i<h;i++){
            scanf("%s",grid[i]);
            for(int j=0;grid[i][j]!='\0';j++){
                if(grid[i][j]=='A')
                    start.push_back(cd(i,j));
            }
        }
        maxlen = 0;
        memset(visited,0,sizeof(visited));
        memset(ans,0,sizeof(ans));
        if(start.empty()){
            printf("Case %d: 0\n",t++);
            continue;
        }
        for(int i=0;i<start.size();i++)
        {
            a = start[i].first;
            b = start[i].second;
            DFS(a,b);
            if(maxlen<ans[a][b])
                maxlen = ans[a][b];
            if(maxlen==26)
                break;
        }
        printf("Case %d: %hd\n",t++,maxlen);
    }
    return 0;
}
