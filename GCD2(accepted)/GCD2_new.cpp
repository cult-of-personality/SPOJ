#include <iostream>
#include <vector>
#include <cstring>
using namespace std;
int euclid(int a,int b)
{
    if(b==0)
        return a;
    else return euclid(b,a%b);
}
int main(void)
{
    vector<int> q;
    int num,dividend,counter,t;
    char str[251];
    cin>>t;
    for(int j=0;j<t;j++)
    {
        cin>>num>>str;
    dividend = 0;   counter = 0;
    if(num==0)
    {
      cout<<str<<endl;
      continue;
    }
    int len = strlen(str);
    q.clear();
    for(int i=0;i<len;i++)
        q.push_back(((int)str[i]-48));
    /* for(int i=0;i<len;i++)
        cout<<q[i]; */
    while(counter<len)
    {
        while((dividend<num)&&(counter<len))
        {
            dividend = 10*dividend + q[counter];
            counter++;
        }
        dividend = dividend%num;
    }
/*  cout<<"Dividend is : "<<dividend;	*/
    cout<<euclid(num,dividend)<<endl;
    }
    return 0;
}
