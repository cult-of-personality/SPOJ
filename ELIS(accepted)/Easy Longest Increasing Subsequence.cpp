#include <iostream>
#include <vector>
using namespace std;

int main(void)
{
	int n,max;
	  cin>>n;
	vector<int> arr(n);
	vector<int> val(n,1);
	for(int i=0;i<n;i++)
	  cin>>arr[i];
	for(int i =0;i<n;i++)
	{
	  for(int j = 0;j<i;j++)
	  {
		if(arr[i]>arr[j])
		{
		  if(val[j]+1>val[i])
			val[i] = val[j] + 1;
		}
	  }
	}
	max = val[0];
	for(int i =1;i<n;i++)
	{
	  if(val[i]>max)
		max = val[i];
	}
	cout<<max<<endl;
	return 0;
}
