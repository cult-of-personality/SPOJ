#include <stdio.h>
#define prime 10000007
int arr[101][101];
void build_table(int );
int main(void) {
	int i,j;
	int n,r;
	scanf("%d %d",&n,&r);
	if(n<r)
		printf("-1");
	else {
		if(n==r)
			printf("1");
		else{
		build_table(n);
		printf("%d",arr[n-1][r-1]);
		/*for(i=0;i<=10;i++)
			printf("%d ",arr[][i]);
		printf("\n");*/
		}
	}
	return 0;
}

void build_table(int n)
{
	/*	need till [n-1][r-1]	*/
	/*	[x][0] = 1 	[x][x] = 1	*/
	/*	if n is even calculate 0 to n/2, copy the rest	*/
	/*	if n is odd, calculate 0 to (n+1)/2, copy the rest*/
	int i,j;
	for(i=0;i<=n-1;i++)
		arr[i][0] = 1;
	for(i=1;i<=n-1;i++)
		arr[i][1] = i;
	for(i=2;i<=n-1;i++)
	{
		if(i%2==0){
			for(j=2;j<=i/2;j++)
				arr[i][j] = (arr[i-1][j]%prime + arr[i-1][j-1]%prime)%prime;
		}
		else{
			for(j=2;j<=(i+1)/2;j++)
				arr[i][j] = (arr[i-1][j]%prime + arr[i-1][j-1]%prime)%prime;
		}
		while(j<=i){
			arr[i][j] = arr[i][i-j];
			j++;
		}
	}
}
