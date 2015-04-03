#include <iostream>
#include <vector>
#include <string>
using namespace std;

int min(int x,int y)
{
	return x>y?y:x;
}

int main(void)
{
	string a,b;
	int t;
	cin>>t;
	for(int i=0;i<t;i++){
		cin>>a;
		cin>>b;
//		int arr[a.length()+1][b.length()+1];
		vector<vector<int> > arr(a.length()+1,vector<int>(b.length()+1,0));
		unsigned int j,k;
		for(j=0;j<=b.length();j++)
			arr[0][j] = j;
		for(j=0;j<=a.length();j++)
			arr[j][0] = j;
		for(j=1;j<=a.length();j++){
			for(k=1;k<=b.length();k++)
			{
				if(a[j-1]==b[k-1])
					arr[j][k] = arr[j-1][k-1];
				else
					arr[j][k] = min(arr[j-1][k],min(arr[j][k-1],arr[j-1][k-1])) + 1;
			}
		}
		cout<<arr[a.length()][b.length()]<<endl;
		arr.clear();
	}
//	cout<<"Exiting"<<endl;
	return 0;
}
