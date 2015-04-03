/* Simple BFS Implementation. Each knight can move in a maximum of 8 directions. */
#include <cstdio>
#include <queue>
using namespace std;
#define INF 1000000000

typedef pair<short int,short int> coord;
int near[9][9];
bool visited[9][9];
coord start,end;

short int process(char c){
    return ((short int)(c-'a') + 1);
}

bool exists(coord c)
{
    if((c.first>0)&&(c.second<=8))
    {
        if((c.second>0)&&(c.second<=8))
            return true;
    }
    return false;
}

void bfs()
{
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++){
            near[i][j] = INF;
            visited[i][j] = false;
        }
    }
    near[start.first][start.second] = 0;
    visited[start.first][start.second] = true;
    queue<coord> q;
    q.push(start);
    int dist;
    coord x,y;
    int s[] = {2,-2};
    int t[] = {1,-1};
    while(!q.empty())
    {
        x = q.front();
        dist = near[x.first][x.second]+1;
        for(int i=0;i<2;i++){
            y.first = x.first + s[i];
            for(int j=0;j<2;j++){
                y.second = x.second + t[j];
                if((exists(y))&&(!visited[y.first][y.second]))
                {
                    if(dist<near[y.first][y.second]){
                        near[y.first][y.second] = dist;
                        visited[y.first][y.second] = true;
                        q.push(y);
                    }
                    if(y==end)
                        return;
                }
            }
        }
        for(int i=0;i<2;i++){
            y.first = x.first + t[i];
            for(int j=0;j<2;j++){
                y.second = x.second + s[j];
                if((exists(y))&&(!visited[y.first][y.second]))
                {
                    if(dist<near[y.first][y.second]){
                        near[y.first][y.second] = dist;
                        visited[y.first][y.second] = true;
                        q.push(y);
                    }
                    if(y==end)
                        return;
                }
            }
        }
        q.pop();
    }
}

int main(void)
{
    char ch;
    short int dig;
    int t;
    scanf(" %d",&t);
    while(t--)
    {
        scanf(" %c %hd",&ch,&dig);
        start.first = process(ch);
        start.second = dig;
        scanf(" %c %hd",&ch,&dig);
        end.first = process(ch);
        end.second = dig;
        if(start==end)
            printf("0\n");
        else{
            bfs();
            printf("%d\n",near[end.first][end.second]);
        }
    }
    return 0;
}
