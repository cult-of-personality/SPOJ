/* BFS inside a grid. From each free point you can move in 4 directions. */
#include <cstdio>
#include <queue>
using namespace std;

#define INF 1000000000
typedef pair<short int,short int> coord;
coord s,d;
int w,h;
char m[25][30];
int near[25][30];

bool exists(coord C)
{
    if((C.first>=0)&&(C.first<h)){
        if((C.second>=0)&&(C.second<w))
            return true;
    }
    return false;
}

inline int process(char ch)
{
    return (int)ch - 48;
}

void perform(coord a,queue<coord> &q,int dist)
{
    if(exists(a))
    {
        if(m[a.first][a.second]=='X')
            return;
        if((a.first==d.first)&&(a.second==d.second)){
            if(dist<near[a.first][a.second])
                near[a.first][a.second] = dist;
            return;
        }
        if((m[a.first][a.second]>='1')&&(m[a.first][a.second]<='9'))
        {
            int w = process(m[a.first][a.second]);
            if((dist+w)<near[a.first][a.second]){
                near[a.first][a.second] = dist+w;
                q.push(a);
            }
        }
    }
}

void bfs()
{
    queue<coord> q;
    q.push(s);
    coord a,b;
    int dist;
    while(!q.empty())
    {
        b = q.front();
        dist = near[b.first][b.second];
        a = coord(b.first+1,b.second);
            perform(a,q,dist);
        a = coord(b.first,b.second+1);
            perform(a,q,dist);
        a = coord(b.first-1,b.second);
            perform(a,q,dist);
        a = coord(b.first,b.second-1);
            perform(a,q,dist);
        q.pop();
    }
}

int main(void)
{
    while(true){
        scanf(" %d %d",&w,&h);
        if((w==0)&&(h==0))
            break;
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                scanf(" %c",&m[i][j]);
                if(m[i][j]=='S'){
                    s.first = i;
                    s.second = j;
                    near[s.first][s.second] = 0;
                }
                else{
                    if(m[i][j]=='D'){
                        d.first = i;
                        d.second = j;
                    }
                    near[i][j] = INF;
                }
            }
        }
        //printf("S = %d %d\n",s.first,s.second);
        //printf("D = %d %d\n",d.first,d.second);
        bfs();
        printf("%d\n",near[d.first][d.second]);
/*        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++)
                printf("%d\t",near[i][j]);
            printf("\n");
        }
*/  }
    return 0;
}
