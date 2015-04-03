#include <iostream>
#include <string>
#include <vector>
using namespace std;
int process(string &s);
int main() {
	string str1;
	string str2 = "0";
	while(str1!=str2)
	{
		cin>>str1;
		if(str1==str2)
			break;
		cout<<process(str1)<<endl;
	}
	return 0;
}

int process(string &s)
{
	vector<int> q(s.length());
	for(string::size_type i =0;i<s.length();i++)
		q[i] = ((int)s[i]-48);
	vector<int> ans(q.size());
	for(unsigned int i=0;i<q.size();i++)
	{
		if(i<2)
		{
			switch(i)
			{
				case 0: ans[0]=1;
						break;
				case 1: if(q[1]==0)
						{
							if((10*q[0]+q[1])<=26)
								ans[1]=ans[0];
								ans[2]=-1;
						}
						else if((10*q[0]+q[1])<=26)
								ans[1]=2;
						else ans[1]=1;
			}
		}
		else{
			if(q[i]==0)
			{
				if(10*q[i-1]+q[i]<=26)
					ans[i] = ans[i-2];
				if(i+1<q.size())
					ans[i+1]=-1;
			}
			else if(ans[i]==-1)	//means preceding digit = 0
				ans[i] = ans[i-1];
			else if(10*q[i-1]+q[i]<=26)
				ans[i]=ans[i-1]+ans[i-2];
			else ans[i]=ans[i-1];
		}
	}
	return ans[ans.size()-1];
}