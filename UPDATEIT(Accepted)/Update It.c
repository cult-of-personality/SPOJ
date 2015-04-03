/* Use Binary Indexed Tree. It is always numbered from 1 to N.     */
/* Range Update, Point Query */
#include <stdio.h>

long long int tree[10001];
int powers[] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192};

void update_interval(int maxVal,int l,int r,int val);
void update_single(int maxVal,int idx,int val);
long long int query(int idx);
void flush(int maxVal);

int main(void)
{
    int i,t;
    int j,n,q,u,l,r,val,idx;
    scanf("%d",&t);
    for(i=0;i<t;i++)
    {
        scanf("%d %d",&n,&u);
        flush(n);
        for(j=0;j<u;j++){
            scanf("%d %d %d",&l,&r,&val);
            update_interval(n,l+1,r+1,val);
        }
        scanf("%d",&q);
        for(j=0;j<q;j++)
        {
            scanf("%d",&idx);
            printf("%lld\n",query(idx+1));
        }
        /*for(j=0;j<=n;j++)
            printf("tree[%d] = %lld\n",j,tree[j]);*/
    }
    return 0;
}

void flush(int n)
{
    int i;
    for(i=0;i<=n;i++)
        tree[i] = 0;
}

void update_interval(int maxVal,int l,int r,int val)
{
    update_single(maxVal,l,val);
    if(r<maxVal)
    	update_single(maxVal,r+1,-val);
}

void update_single(int maxVal,int idx,int val)
{
    while(idx<=maxVal)
    {
        tree[idx] += val;
        idx += (idx&-idx);
    }
}

long long int query(int idx)
{
    //need to find a common ancestor for idx & idx-1
    long long int a = 0;
    int x = idx;
    while(x>0){
        a += tree[x];
        x -= (x&-x);
    }
    return a;
}
