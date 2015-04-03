#include <stdio.h>

typedef struct
{
   int p;
   int q;
}element;

void propagate(element arr[],int n)
{
   int i;
   for(i=0;i<n-1;i++)
   {
      arr[i+1].p=0;	arr[i+1].q=0;
      arr[i+1].p += (arr[i].p + 2*arr[i].q);
      arr[i+1].q += (arr[i].p + arr[i].q);
   }
}

int main(void )
{
   int n;
    scanf("%d",&n);
   if(n==0)
   {
      printf("1\n");
      return 0;
   }
   if(n==1)
   {
      printf("3\n");
      return 0;
   }
     element a[30],b[30];
	a[0].p=1;	a[0].q=0;
	b[0].p=0;	b[0].q=1;
     propagate(a,n);
     propagate(b,n);
     int result = 0;
     result += 2*(2*a[n-2].p + b[n-2].p);
     result += 3*(2*a[n-2].q + b[n-2].q);
     printf("%d\n",result);
     return 0;
}
