#include <stdio.h>
#include <stdlib.h>

long long int *one,*two;
int n;

void update_single(long long int ft[],int idx,long long int v);
void update_interval(int l,int r,long long int v);
long long int query_single(long long int ft[],int l);
long long int query_interval(int l,int r);

int main(void)
{
    int t;
    int idx,l,r;
    int i,c;
    short int op;
    long long int val;
    scanf("%d",&t);
    while(t--){
        scanf("%d %d",&n,&c);
        one = (long long int*)calloc(n+1,sizeof(long long int));        //calloc initialises the array to zero.
        two = (long long int*)calloc(n+1,sizeof(long long int));        //could have used memset too. But that needs string.h
        for(i=0;i<c;i++){
            scanf("%hd",&op);
            switch(op)
            {
              case 0: scanf("%d %d %lld",&l,&r,&val);
                      update_interval(l,r,val);
                      break;
              case 1: scanf("%d %d",&l,&r);
                      printf("%lld\n",query_interval(l,r));
                      break;
            }
        }
        free(one);
        free(two);
    }
    return 0;
}

void update_single(long long int ft[],int idx,long long int v)
{
    while(idx<=n){
        ft[idx] += v;
        idx += (idx&-idx);
    }
}

void update_interval(int a,int b,long long int v)
{
    update_single(one,a,v);
    update_single(one,b+1,-v);
    update_single(two,a,v*(a-1));
    update_single(two,b+1,-v*b);
}

long long int query_single(long long int ft[],int idx)
{
    long long int sum = 0;
    while(idx>0){
        sum += ft[idx];
        idx -= (idx&-idx);
    }
    return sum;
}

long long int query_interval(int a,int b)
{
    long long int x = b*query_single(one,b) - query_single(two,b);
    long long int y = (a-1)*query_single(one,a-1) - query_single(two,a-1);
    return (x-y);
}
