#include <cstdio>
#include <vector>
#include <queue>
using namespace std;
#define INF 10000
/*	BFS from each white vertex throughout the grid or till new_dist<=old_dist.*/

typedef pair<short int,short int> coord;
vector<coord> white;
char arr[185][185];
short int nearest[185][185];
coord s;
short int n,m;

void bfs(coord c);

inline short int absolute(short int num){
    return (num>=0)?num:(-num);
}
inline short int calc_dist_to_source(coord c){
    return absolute(s.first-c.first) + absolute(s.second-c.second);
}
inline bool exists(coord c){
    if((c.first<n)&&(c.second<m))
        return true;
    return false;
}
inline void perform(coord d,queue<coord> &q){
    if(exists(d)){
        if(calc_dist_to_source(d)<nearest[d.first][d.second]){
            q.push(d);
            nearest[d.first][d.second] = calc_dist_to_source(d);
        }
    }
}

int main(void)
{
	int t;
	short int j,k;
	scanf("%d",&t);
    char ch;
	for(int i=0;i<t;i++)
	{
        white.clear();
		scanf("%hd %hd",&n,&m);
        for(j=0;j<n;j++){
			
				scanf("%s",arr[j]);
            for(k=0;k<m;k++){
				if(arr[j][k]=='1'){         //set nearest dist of all whites to 0, infinity otherwise.
                  white.push_back(coord(j,k));
                  nearest[j][k] = 0;
                }
                else nearest[j][k] = INF;
		    }
       //     scanf("%c",&ch);
		}
/*      for(int p=0;p<white.size();p++)
            printf("%hd %hd\n",white[p].first,white[p].second);
        //BFS from all white pixels.
*/      for(int p=0;p<white.size();p++){
            s = white[p];
            bfs(white[p]);
        }
        for(j=0;j<n;j++){
            for(k=0;k<m;k++)
                printf("%hd ",nearest[j][k]);
            printf("\n");
        }
	}
}

void bfs(coord c)
{
    queue<coord> q;
    q.push(c);
    coord d;
    while(!q.empty()){
        c = q.front();
        d = coord(c.first+1,c.second);
            perform(d,q);
        d = coord(c.first,c.second+1);
            perform(d,q);
        d = coord(c.first-1,c.second);
            perform(d,q);
        d = coord(c.first,c.second-1);
            perform(d,q);
        q.pop();
    }
}
