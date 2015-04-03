/* Segment Trees */
#include <stdio.h>

typedef struct{
	int first;
	int second;
}element;

element M[263000];
int A[100001]={0};
int start,end,idx;

int max(int a,int b){
	return (a>=b)?a:b;
}

element combine(element *l,element *r)
{
	element e;
	if(l->first>=r->first){
		e.first = l->first;
		e.second = max(l->second,r->first);
	}
	else{
		e.first = r->first;
		e.second = max(l->first,r->second);
	}
	return e;
}

void initialise(int node,int b,int e)
{
	if(b==e){
		M[node].first = A[b];
		M[node].second = -1;
	}
	else{
		int mid = (b+e)>>1;
		int left = node<<1;
		int right = left+1;
		initialise(left,b,mid);
		initialise(right,mid+1,e);
		M[node] = combine(&M[left],&M[right]);
	}
}

void update(int node,int b,int e){
	if((b==e)&&(b==idx)){
		M[node].first = A[idx];
		M[node].second = -1;
	}
	else{
		int mid = (b+e)>>1;
		int left = node<<1;
		int right = left+1;
		if(idx<=mid)
			update(left,b,mid);
		else update(right,mid+1,e);
		M[node] = combine(&M[left],&M[right]);
	}
}

element query(int node,int b,int e)
{
	if((start<=b)&&(e<=end))
		return M[node];
	int mid = (b+e)>>1;
	int left = node<<1;
	int right = left+1;
	if(end<=mid)
		return query(left,b,mid);
	if(start>mid)
		return query(right,mid+1,e);
	element p = query(left,b,mid);
	element q = query(right,mid+1,e);
	return combine(&p,&q);
}

int main(void) {
	int n,m,i;
	char ch;
	scanf(" %d",&n);
	for(i=1;i<=n;i++)
		scanf("%d",&A[i]);
	initialise(1,1,n);
	scanf(" %d",&m);
	for(i=0;i<m;i++){
		scanf(" %c",&ch);
		switch(ch){
			case 'U': scanf(" %d",&idx);
					  scanf(" %d",&A[idx]);
					  update(1,1,n);
					  break;
			case 'Q': scanf(" %d %d",&start,&end);
					  element e = query(1,1,n);
					  printf("%d\n",e.first+e.second);
					  break;
		}
	}
	return 0;
}
