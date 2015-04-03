#include <cstdio>
#include <vector>
using namespace std;

int min(int a,int b){   return (a<b)?a:b;   }
int max(int a,int b){   return (a>b)?a:b;   }

int main(void)
{
    int x,y,z;
    int a,b;
    int t;
    scanf("%d",&t);
    vector<int> modulo;
    for(int i=0;i<t;i++)
    {
        scanf("%d %d %d",&x,&y,&z);
        if((z>x)&&(z>y)){
            printf("NO\n");
            continue;
        }
        a = min(x,y);
        b = max(x,y);
        modulo.clear();
        if(z%a==0){
            printf("YES\n");
            continue;
        }
        modulo.push_back(a-b%a);
        int num;
        while(num!=modulo[0]){
            num = a - (b-num)%a;
            if(num==modulo[0])
                break;
            else{
               modulo.push_back(num);
            }
        }
        int j;
        for(j=0;j<modulo.size();j++)
        {
            if((z-modulo[j])%a==0){
                printf("YES\n");
                break;}
        }
        if(j<modulo.size())
            continue;
        else printf("NO\n");  
    }
    return 0;
}
