/* Segment Trees, Lazy Propagation */
#include <cstdio>
using namespace std;

struct element{
    int on;
    int off;
    bool update;
};

element M[263000];
element def = {0,1,false};
int start,end;

inline void complement(int idx)
{
    M[idx].update = !M[idx].update;
}

inline void swap(int idx)
{
    int temp = M[idx].on;
    M[idx].on = M[idx].off;
    M[idx].off = temp;
}

inline void unflag(int idx,bool mode)
{
    if(M[idx].update)
    {
        swap(idx);
        if(mode){
            complement(idx<<1);
            complement((idx<<1)+1);
        }
        M[idx].update = false;
    }
}

void initialise(int node,int b,int e)
{
    if(b==e){
        M[node] = def;
    }
    else{
        int mid = (b+e)>>1;
        int left = node<<1;
        int right = left+1;
        initialise(left,b,mid);
        initialise(right,mid+1,e);
        M[node].on = 0;
        M[node].off = M[left].off + M[right].off;
        M[node].update = false;
    }
}

void update_op(int node,int b,int e)
{
    if((b>end)||(e<start))
        return;
    int mid = (b+e)>>1;
    int left = node<<1;
    int right = left+1;
    if((start<=b)&&(e<=end))
    {
        complement(node);
        if(b!=e)
            unflag(node,true);
        else unflag(node,false);
    }
    else{
        update_op(left,b,mid);
        update_op(right,mid+1,e);
        //merge the results.
        if(M[left].update){
            if(b!=mid)
                unflag(left,true);
            else unflag(left,false);
        }
        if(M[right].update){
            if(mid!=e-1)
                unflag(right,true);
            else unflag(right,false);
        }
        M[node].on = M[left].on + M[right].on;
        M[node].off = M[left].off + M[right].off;
     }
}

int query(int node,int b,int e)
{
    if((b>end)||(e<start))
        return 0;
    int mid = (b+e)>>1;
    int left = node<<1;
    int right = left+1;
    if((start<=b)&&(e<=end)){
        if(M[node].update)
            return M[node].off;
        else return M[node].on;
    }
    if(M[node].update)
        unflag(node,true);
    return query(left,b,mid) + query(right,mid+1,e);
}

int main(void)
{
    int n,m;
    short int op;
    scanf("%d %d",&n,&m);
    initialise(1,1,n);
    for(int i=0;i<m;i++)
    {
        scanf("%hd %d %d",&op,&start,&end);
        switch(op)
        {
            case 0: update_op(1,1,n);
                    break;
            case 1: printf("%d\n",query(1,1,n));
        }
    }
    return 0;
}
