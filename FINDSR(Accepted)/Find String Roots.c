#include <stdio.h>
#include <string.h>

char str[100002];
int F[100001];

void failure_function(int len)
{
    int i,j;
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
    }
}

int main(void)
{
    int len,d;
    while(1){
        scanf("%s",str);
        if(str[0]=='*')
            break;
        len = strlen(str);
        failure_function(len);
        if(F[len]!=0){
            d = len - F[len];
            if(len%d==0)
                printf("%d\n",len/d);
            else printf("1\n");
        }
        else printf("1\n");
    }
    return 0;
}
