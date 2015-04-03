#include <stdio.h>

typedef struct{
	int left;
	int right;
}element;

char str[30001];
element M[65600];
void initialise(int node,int b,int e);
void update(int node,int b,int e,int idx);
int check();

int main(void)
{
	short int i;
	int j,n,m,k;
	for(i=0;i<10;i++)
	{
		scanf("%d",&n);
		scanf("%s",str);
		initialise(1,0,n-1);
		scanf("%d",&m);
		printf("Test %d:\n",i+1);
		for(j=0;j<m;j++)
		{
			scanf("%d",&k);
			if(k==0){
				switch(check())
				{
					case 0:	printf("YES\n");
							break;
					case 1: printf("NO\n");
							break;
				}
			}
			else update(1,0,n-1,k-1);
		}
		/*for(j=0;j<16;j++)
			printf("M[%d].left = %d  M[%d].right = %d\n",j,M[j].left,j,M[j].right);*/
	}
	return 0;
}

void initialise(int node,int b,int e)
{
	if(b==e){		//initialise a leaf
		if(str[b]=='('){		//leaf is a left bracket.
			M[node].left = 1;
			M[node].right = 0;
		}
		else{					//leaf is a right bracket.
			M[node].left = 0;
			M[node].right = 1;
		}
	}
	else{
		int mid = (b+e)>>1;
		int left_leaf = 2*node;
		int right_leaf = 2*node+1;
		initialise(left_leaf,b,mid);
		initialise(right_leaf,mid+1,e);
		//merge results of children now.
		if(M[left_leaf].left >= M[right_leaf].right){
			M[node].left = M[left_leaf].left - M[right_leaf].right;
			M[node].left += M[right_leaf].left;
			M[node].right = M[left_leaf].right;
		}
		else{
			M[node].right = M[right_leaf].right - M[left_leaf].left;
			M[node].right += M[left_leaf].right;
			M[node].left = M[right_leaf].left;
		}
	}
}

int check()
{
	if((M[1].left==0)&&(M[1].right==0))
		return 0;
	return 1;
}

void update(int node,int b,int e,int idx)
{
	if((b==e)&&(b==idx)){
		if(str[b]=='('){
			str[b] = ')';
			M[node].left = 0;
			M[node].right = 1;
		}
		else{
			str[b] = '(';
			M[node].left = 1;
			M[node].right = 0;
		}
	}
	else{
		//recurse down to the leaf.
		int mid = (b+e)>>1;
		int left_leaf = 2*node;
		int right_leaf = 2*node+1;
		if(idx<=mid)
			update(left_leaf,b,mid,idx);
		else update(right_leaf,mid+1,e,idx);
		//merge results of children now.
		if(M[left_leaf].left>=M[right_leaf].right){
			M[node].left = M[left_leaf].left - M[right_leaf].right;
			M[node].left += M[right_leaf].left;
			M[node].right = M[left_leaf].right;
		}
		else{
			M[node].right = M[right_leaf].right - M[left_leaf].left;
			M[node].right += M[left_leaf].right;
			M[node].left = M[right_leaf].left;
		}
	}
}
