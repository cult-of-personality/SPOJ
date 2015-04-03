#include <stdio.h>
//Size of heap = 2n-1
int A[100000];
int M[270000];
void initialize(int node,int b,int e);
int query(int node,int b,int e,int beg,int end);

int main(void) {
	int i,t,n,q,j,beg,end,res;
	scanf("%d",&t);
	for(i=0;i<t;i++)
	{
		scanf("%d %d",&n,&q);
		for(j=0;j<n;j++)
			scanf("%d",&A[j]);
		initialize(1,0,n-1);
		printf("Scenario #%d:\n\n",i+1);
		for(j=0;j<q;j++){
			scanf("%d %d",&beg,&end);
			res = query(1,0,n-1,beg-1,end-1);
			printf("%d\n",A[res]);
		}
		printf("\n");
	}
	return 0;
}

void initialize(int node,int b,int e)
{
	if(b==e)
		M[node] = b;
	else{
		int mid = (b+e)>>1;
		initialize(2*node,b,mid);
		initialize(2*node+1,mid+1,e);
		if(A[M[2*node]]<=A[M[2*node+1]])
			M[node] = M[2*node];
		else M[node] = M[2*node+1];
	}
}

int query(int node,int b,int e,int beg,int end)
{
	if((beg>e)||(end<b))
		return -1;
	if((b>=beg)&&(e<=end))
		return M[node];
	int p,q;
	int mid = (b+e)>>1;
	p = query(2*node,b,mid,beg,end);
	q = query(2*node+1,mid+1,e,beg,end);
	if(p==-1)
		return q;
	if(q==-1)
		return p;
	if(A[p]<=A[q])
		return p;
	else return q;
}
