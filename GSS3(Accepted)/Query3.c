#include <stdio.h>

typedef struct{
    int sum;
    int best;
    int left;
    int right;
}element;

short int A[50000];
element M[131200];

inline int max(int a,int b){
    return (a>b)?a:b;
}

element combine(element *l,element *r)
{
    element e;
    e.left = l->left;
    if(l->sum + r->left > e.left)
        e.left = l->sum + r->left;
    e.right = r->right;
    if(l->right + r->sum > e.right)
        e.right = l->right + r->sum;
    e.best = max(l->right + r->left, max(l->best,r->best));
    e.sum = l->sum + r->sum;
    return e;
}

void initialise(int node,int b,int e)
{
    if(b==e){
        M[node].sum = A[b];
        M[node].best = A[b];
        M[node].left = A[b];
        M[node].right = A[b];
    }
    else{
        int mid = (b+e)>>1;
        int left_child = node<<1;
        int right_child = left_child+1;
        initialise(left_child,b,mid);
        initialise(right_child,mid+1,e);
        M[node] = combine(&M[left_child],&M[right_child]);
    }
}
element query(int node,int b,int e,int l,int r)
{
    if((l<=b)&&(e<=r))
        return M[node];
    int mid = (b+e)>>1;
    int left_child = node<<1;
    int right_child = left_child+1;
    if(r<=mid)
        return query(left_child,b,mid,l,r);
    if(l>mid)
        return query(right_child,mid+1,e,l,r);
    element p = query(left_child,b,mid,l,r);
    element q = query(right_child,mid+1,e,l,r);
    return combine(&p,&q);
}

void update(int node,int b,int e,int idx)
{
    if((b==e)&&(b==idx))
    {
        M[node].sum = A[idx];
        M[node].best = A[idx];
        M[node].left = A[idx];
        M[node].right = A[idx];
    }
    else{
        int mid = (b+e)>>1;
        int left_child = node<<1;
        int right_child = left_child+1;
        if(idx<=mid)
            update(left_child,b,mid,idx);
        else update(right_child,mid+1,e,idx);
        M[node] = combine(&M[left_child],&M[right_child]);
    }
}

int main(void)
{
    int i,n,m,l,r,idx;
    short int op,val;
    element e;
    scanf("%d",&n);
    for(i=0;i<n;i++)
        scanf("%hd",&A[i]);
    initialise(1,0,n-1);
    scanf("%d",&m);
    for(i=0;i<m;i++)
    {
        scanf("%hd",&op);
        switch(op)
        {
            case 0: scanf("%d %hd",&idx,&val);
                    A[idx-1] = val;
                    idx--;
                    update(1,0,n-1,idx);
                    break;
            case 1: scanf("%d %d",&l,&r);
                    e = query(1,0,n-1,l-1,r-1);
                    printf("%d\n",e.best);
                    break;
        }
    }
    return 0;
}
