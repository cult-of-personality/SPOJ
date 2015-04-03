#include <iostream>
#include <string>
using namespace std;

int F[200002];

void construct_str(string &str,string &S)
{
	for(int i=str.length()-1;i>=0;i--)
		S += str[i];
	S += '#';
	S += str;
}

void failure_func(string &S,int len)
 {
 	F[0] = 0;
 	F[1] = 0;
 	int j;
  	for(int i=2;i<=len;i++)
 	{
 		j = F[i-1];
 		while(true){
 			if(S[i-1]==S[j]){
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

int main(void) {
	string str;
	int max,str_len,S_len;
	while(getline(cin,str))
	{
		string S;
		construct_str(str,S);
		S_len = (int)S.length();
		str_len = (int)str.length();
		failure_func(S,S_len);
		//output left;
		max = F[S_len];
		cout<<str;
		for(int i=str_len-max-1;i>=0;i--)
			cout<<str[i];
		cout<<"\n";
	}
	return 0;
}
