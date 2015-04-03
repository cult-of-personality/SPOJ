#include <iostream>
#include <vector>
#include <string>
using namespace std;

long long int power(int ,int );
int get_length(long long int);
long long int get_actual(long long int,int);
char Map(int);

int main() {
	int t,len;
	long long int n,actual;
	cin>>t;
	for(int i=0;i<t;i++)
	{
		cin>>n;
		len = get_length(n);
		actual = get_actual(n,len);
		string str;
		int j = len-1;
		while(j>=0)
		{
			str += Map(actual/(long long int)power(5,j));
			actual %= (long long int)power(5,j);
			j--;
		}
		cout<<str<<endl;
	}
	return 0;
}

char Map(int num)
{
	switch(num){
		case 0: return 'm';
		case 1: return 'a';
		case 2: return 'n';
		case 3: return 'k';
		case 4: return 'u';
	}
}

int get_length(long long int num)
{
	int j=0;
	while(num>0){
		j++;
		num -= (long long int)power(5,j);
	}
	return j;
}

long long int get_actual(long long int num,int len)
{
	int j=1;
	long long int actual;
	actual = num - 5*(((long long int)power(5,len-1)-1)/4);
	return actual-1;
}

long long int power(int base_num, int exponent){
	long long int prod = 1;
	for(int i=0;i<exponent;i++)
		prod*=base_num;
	return prod;
}