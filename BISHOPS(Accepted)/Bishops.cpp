#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main(void)
{
	string str;
	vector<int> v;
	while(cin>>str){
	  v.clear();
	  for(int i=0;i<str.length();i++)
		v.push_back((int)str[i]-48);
	  if(v.size()==1){
		switch(v[0]){
		  case 1: cout<<"1"<<endl;
				  continue;
		  case 2: cout<<"2"<<endl;
				  continue;
		}
	  }
	  int carry = 0;
	  for(int i=v.size()-1;i>=0;i--)
	  {
		/*	Mulitply N by 2	*/
		v[i] *= 2;
		v[i] += carry;
		carry = v[i]/10;
		v[i] = v[i]%10;
	  }
	  if(carry!=0){
		v.resize(v.size()+1);
		for(int i=v.size()-2;i>=0;i--)
		  v[i+1] = v[i];
		v[0] = carry;
	  }
/*	Subtract 2	*/
		v[v.size()-1]-=2;
		for(int i=v.size()-1;i>=0;i--)
		{
		  if(v[i]<0){
			v[i]+=10;
			v[i-1]--;
		  }
		}
        int i = 0;
        while(v[i]==0)
            i++;
		while(i<v.size()){
		  cout<<v[i];
		  i++;
		}
		cout<<endl;
	}
	return 0;
}
