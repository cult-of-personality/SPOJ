#include <stdio.h>

char str[1000002];
int len;
int F[1000001];

void failure_function()
{
    int i,j,d;
    F[0] = 0;
    F[1] = 0;
    for(i=2;i<=len;i++)
    {
        j = F[i-1];
        while(1)
        {
            if(str[i-1]==str[j]){
                F[i] = j+1;
                break;
            }
            if(j==0){
                F[i] = 0;
                break;
            }
            j = F[j];
        }
        if(F[i]!=0){
            d = i - F[i];
            if(i%d==0)
                printf("%d %d\n",i,i/d);
        }
    }
}

int main(void)
{
    short int i,t;
    scanf("%hd",&t);
    for(i=0;i<t;i++)
    {
        scanf("%d",&len);
        scanf("%s",str);
        printf("Test case #%hd\n",i+1);
        failure_function();
        printf("\n");
    }
    return 0;
}
