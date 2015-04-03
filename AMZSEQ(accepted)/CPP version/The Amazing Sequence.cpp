#include <iostream>
#include <vector>
using namespace std;
typedef struct
{
    int count_2;
    int count_3;
}element;

void propagate(vector<element>& arr,int );

int main(void )
{
   int n;
     cin>>n;
   if(n==0){
     cout<<"1";
     return 0;
   }
   if(n==1)
   {
     cout<<"3";
     return 0;
   }
   vector<element> a(n);
   vector<element> b(n);
   vector<element> c(n);
	a[0].count_2 = 1;    a[0].count_3 = 0;
	b[0].count_2 = 0;    b[0].count_3 = 1;
        c[0].count_2 = 1;    c[0].count_3 = 0;
   propagate(a,n);
   propagate(b,n);
   propagate(c,n);
   int result = 0;
   result += (2*a[n-2].count_2 + 3*a[n-2].count_3);
   result += (2*b[n-2].count_2 + 3*b[n-2].count_3);
   result += (2*c[n-2].count_2 + 3*c[n-2].count_3);
	cout<<result<<endl;
   return 0;
}

void propagate(vector<element>& arr,int n)
{
   for(int i=0;i<n-1;i++)
   {
      arr[i+1].count_2 = 0;   arr[i+1].count_3 = 0;
      arr[i+1].count_2 += arr[i].count_2;
      arr[i+1].count_2 += 2*arr[i].count_3;
      arr[i+1].count_3 += arr[i].count_2;
      arr[i+1].count_3 += arr[i].count_3;
   }
}
