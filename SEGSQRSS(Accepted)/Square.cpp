/* Segment Trees. Lazy Propagation.*/
#include <cstdio>
using namespace std;

struct element{
    long long sqsum;
    int sum;
    int uval,sval;
    bool sflag;
};

element M[270000];
short int A[100001]={0};
int st,end,val;

element combine(element a,element b)
{   //check & rewrite.
    element res;
    res.sqsum = a.sqsum + b.sqsum;
    res.sum = a.sum + b.sum;
    res.uval = 0;
    res.sval = 0;
    res.sflag = false;
    return res;
}

void split(int node,int b,int e)
{
    if(b>e)
        return;
    int mid=(b+e)>>1;
    int left=node<<1;
    int right=left+1;
    int left_range = mid-b+1;
    int right_range = e-mid;
    int term;
    if(M[node].sflag)
    {
        M[node].sflag = false;
        //need to set children.
        if(b!=e){
            term = M[node].sval*M[node].sval;
            M[left].sflag = true;   M[left].uval = 0;
            M[right].sflag = true;  M[right].uval = 0;
            M[left].sqsum = left_range*term;
            M[left].sum = left_range*M[node].sval;
            M[right].sqsum = right_range*term;
            M[right].sum = right_range*M[node].sval;
        }
    }
    //now add.
    if(M[node].uval!=0)
    {
        //put check for b==e (leaf node)
        term = M[node].uval*M[node].uval;
        split(left,b,mid);
        split(right,mid+1,e);
        if(b!=e){
            M[left].sqsum += ((left_range*term)+(2*M[left].sum*M[node].uval));
            M[left].sum += left_range*M[node].uval;
            M[left].uval = M[node].uval;
            M[right].sqsum += ((right_range*term)+(2*M[right].sum*M[node].uval));
            M[right].uval = M[node].uval;
        }
        M[node].uval = 0;
    }
}

void init(int node,int b,int e)
{
    if(b==e){
        M[node].sum = A[b];
        M[node].sqsum = A[b]*A[b];
        M[node].uval = 0;
        M[node].sval = 0;
        M[node].sflag = false;
    }
    else{
        int mid = (b+e)>>1;
        int left = node<<1;
        int right = left+1;
        init(left,b,mid);
        init(right,mid+1,e);
        M[node] = combine(M[left],M[right]);
    }
}

void update(int node,int b,int e)
{
    if((b>end)||(e<st))
        return;
    split(node,b,e);
    int mid = (b+e)>>1;
    int left = node<<1;
    int right = left+1;
    if((st<=b)&&(e<=end))
    {   
        //split(node,b,e);
        M[node].sqsum += (((e-b+1)*val*val) + 2*M[node].sum*val);
        M[node].sum += (e-b+1)*val;
        M[node].uval = val;
    }
    else{
        update(left,b,mid);
        update(right,mid+1,e);
        M[node] = combine(M[left],M[right]);
    }
}

void set(int node,int b,int e)
{
    if((b>end)||(e<st))
        return;
    split(node,b,e);
    int mid = (b+e)>>1;
    int left = node<<1;
    int right = left+1;
    if((st<=b)&&(e<=end)){  
        M[node].sqsum = (e-b+1)*val*val;
        M[node].sum = (e-b+1)*val;
        M[node].uval = 0;
        M[node].sval = val;
        M[node].sflag = true;
    }
    else{
        set(left,b,mid);
        set(right,mid+1,e);
        M[node] = combine(M[left],M[right]);
    }
}

element query(int node,int b,int e)
{
    split(node,b,e);
    if((st<=b)&&(e<=end))
        return M[node];
    int mid=(b+e)>>1;
    int left=node<<1;
    int right=left+1;
    if(end<=mid)
        return query(left,b,mid);
    if(st>mid)
        return query(right,mid+1,e);
    element p = query(left,b,mid);
    element q = query(right,mid+1,e);
    return combine(p,q);
}

int main(void)
{
    short int i,t,op;
    int j,k,n,q;
    element e;
    scanf(" %hd",&t);
    for(i=1;i<=t;i++)
    {
        scanf(" %d %d",&n,&q);
        for(j=1;j<=n;j++)
            scanf("%hd",&A[j]);
        init(1,1,n);
        printf("Case %d:\n",i);
        for(j=0;j<q;j++)
        {
            scanf(" %hd",&op);
            switch(op)
            {
                case 0: scanf(" %d %d %d",&st,&end,&val);
                        set(1,1,n);
                        break;
                case 1: scanf(" %d %d %d",&st,&end,&val);
                        update(1,1,n);
                        break;
                case 2: scanf(" %d %d",&st,&end);
                        e = query(1,1,n);
                        printf("%lld\n",e.sqsum);
                        break;
            }
        }
    }
    return 0;
}
