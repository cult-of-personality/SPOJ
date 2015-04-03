#include <iostream>
#include <stack>
#include <vector>
using namespace std;
int judge(vector<int>& v);
int main() {
	int n;
	vector<int> numbers;
	while(true)
	{
		cin>>n;
		if(n==0)
		 break;
		numbers.clear();
		numbers.resize(n);
		for(int i=0;i<numbers.size();i++)
			cin>>numbers[i];
		switch(judge(numbers))
		{
			case 0: cout<<"no"<<endl;
					break;
			case 1: cout<<"yes"<<endl;
					break;
		}
	}
	return 0;
}

int judge(vector<int>& v)
{
	stack<int> st;
	vector<int> rearrange;
	int i =0;
	while(i<v.size())
	{
		if(v[i]==rearrange.size()+1){
			rearrange.push_back(v[i]);
			i++;
			continue;
		}
		else if((!st.empty())&&(st.top()==rearrange.size()+1)){
			rearrange.push_back(st.top());
			st.pop();
			continue;
		}
		else{
			if(st.empty()){
				st.push(v[i]);
				i++;
			}
			else if(v[i]>st.top())
				return 0;
			else {
				st.push(v[i]);
				i++;
			}
		}
	}
	return 1;
}