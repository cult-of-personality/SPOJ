//problem from second test case onwards. WHY?
#include <cstdio>
#include <map>
#include <string>
using namespace std;

void update(int tree[], int idx,int n)
{
    while(idx<=n)
    {
        tree[idx]++;
        idx += (idx&-idx);
    }
}

int sum(int tree[], int idx)
{
    int x = 0;
    while(idx>0)
    {
        x += tree[idx];
        idx -= (idx&-idx);
    }
    return x;
}

long long int inversion_count(int arr[],int n)
{
    int tree[30001] = {0};
    long long int count = 0;
    for(int i=0;i<n;i++)
    {
        update(tree,arr[i],n);
        count += (i+1 - sum(tree,arr[i]));
    }
    return count;
}

int main(void)
{
    short int t;
    int n;
    scanf("%hd",&t);
    for(short int i=0;i<t;i++)
    {
        map<string,int> messed;
        string s;
        char str[21];
        scanf("%d",&n);
        int index[n];
        for(int j=0;j<n;j++)
        {
            scanf("%s",str);
            s = str;
            messed[s] = j+1;
        }
        for(int j=0;j<n;j++)
        {
            scanf("%s",str);
            s = str;
            index[j] = messed[s];
        }
        //perform inversion count on index.
/*        for(int i=0;i<n;i++)
            cout<<"Index["<<i<<"] = "<<index[i]<<"\n";
*/  
        printf("%lld\n",inversion_count(index,n));
    }
    return 0;
}
