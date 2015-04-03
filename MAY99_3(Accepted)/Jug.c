#include <stdio.h>

int min(int a,int b){
	return (a<b)?a:b;
}

int max(int a,int b){
	return (a>b)?a:b;
}

int main(void)
{
	int i,t;
	int x,y,z;
	int a,b;
	scanf("%d",&t);
	for(i=0;i<t;i++)
	{
		scanf("%d %d %d",&x,&y,&z);
		a = min(x,y);
		b = max(x,y);
		if(z>b){
			printf("NO\n");
			continue;
		}
		if(z%a==0){
			printf("YES\n");
			continue;
		}
		if(z==(b-a)){
			printf("YES\n");
			continue;
		}
		int temp = a - b%a;
		if((z-temp)%a==0){
			printf("YES\n");
			continue;
		}
		printf("NO\n");
	}
	return 0;
}
