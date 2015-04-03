#include <stdio.h>

int main(void )
{
	int arr[31] = {1,1};
	int a[31] = {0,1,4};
	int i,j,t,n;
	scanf("%d",&t);
	for(i=3;i<=30;i++)
	{
		switch(i%2)
		{
			case 0: a[i] = 3;
					break;
			case 1: a[i] = 2;
		}
	}
	for(i=2;i<=25;i++)
	{
		for(j=1;j<=i;j++)
			arr[i] += arr[i-j]*a[j];
	}
	/*for(i=0;i<31;i++)
		printf("%d\n",arr[i]);*/
	for(i=0;i<t;i++){
		scanf("%d",&n);
		printf("%d %d\n",i+1,arr[n]);
	}
	return 0;
}
