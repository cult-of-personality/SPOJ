/* Accepted. 2D BIT. */
#include <stdio.h>
#include <string.h>
#define SI short int 

int tree[1025][1025]={0};

void update_y(SI n,SI x,SI y,int val);
void update(SI n,SI x,SI y,int val);
int query(SI n,SI p,SI q,SI r,SI s);
long long int query_single(SI n,SI x,SI y);

int main(void){
    char op[5];
    char end[] = {'E','N','D','\0'};
    char set[] = {'S','E','T','\0'};
    char sum[] = {'S','U','M','\0'};
    int k,t;
    SI n,p,q,r,s,i,j;
    int val;
    scanf("%d",&t);
    for(k=0;k<t;k++)
    {
        scanf("%hd",&n);
        int matrix[n+1][n+1];
        memset(tree,0,sizeof(tree));
        memset(matrix,0,sizeof(matrix));
        while (1){
            scanf("%s",op);
            if(strcmp(op,end)==0)
                break;
            if(strcmp(op,set)==0){
                //write code for setting a particular value to num in the matrix
                scanf(" %hd %hd %d",&p,&q,&val);
                int diff = val - matrix[p+1][q+1];
                update(n,p+1,q+1,diff);
                matrix[p+1][q+1] = val;
            }
            else if(strcmp(op,sum)==0){
                //use query to find sum of submatrix
                scanf(" %hd %hd %hd %hd",&p,&q,&r,&s);
/*                if((p==r)&&(q==s))
                    printf("%d\n",matrix[p][q]);
*/                printf("%d\n",query(n,p+1,q+1,r+1,s+1));
            }
        }
    }
    return 0;
}

void update_y(SI n,SI x,SI y,int val)
{
    while(y<=n)
    {
        tree[x][y] += val;
        y+=(y&-y);
    }
}

void update(SI n,SI x,SI y,int val)
{
    while(x<=n)
    {
        update_y(n,x,y,val);
        x+=(x&-x);
    }
}

int query(SI n,SI p,SI q,SI r,SI s)
{
    long long int a,b,c,d;
    a = query_single(n,p-1,q-1);
    b = query_single(n,p-1,s);
    c = query_single(n,r,q-1);
    d = query_single(n,r,s);
    return (int)(d-b-c+a);
}

long long int query_single(SI n,SI x,SI y)
{
    long long int res = 0;
    SI i,j;
    for(i=x;i>0;i-=(i&-i))
    {
        for(j=y;j>0;j-=(j&-j))
            res += tree[i][j];
    }
    return res;
}
