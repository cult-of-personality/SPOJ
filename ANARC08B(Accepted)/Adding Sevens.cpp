#include <iostream>
#include <string>
#include <map>
using namespace std;

void separate(string s,int &a,int &b);
void make_lookup();
string transform(int num);
map<string,int> lookup;

int main(void )
{
	string str,s1;
	int a,b;
	make_lookup();
	while(str!="BYE")
	{
		cin>>str;
		if(str=="BYE")
			break;
		separate(str,a,b);
		s1 = transform(a+b);
		cout<<str<<s1<<endl;
	}
	return 0;
}

void make_lookup()
{
	lookup.insert(pair<string,int>("063",0));
	lookup.insert(pair<string,int>("010",1));
	lookup.insert(pair<string,int>("093",2));
	lookup.insert(pair<string,int>("079",3));
	lookup.insert(pair<string,int>("106",4));
	lookup.insert(pair<string,int>("103",5));
	lookup.insert(pair<string,int>("119",6));
	lookup.insert(pair<string,int>("011",7));
	lookup.insert(pair<string,int>("127",8));
	lookup.insert(pair<string,int>("107",9));
}

void separate(string s,int &a,int &b)
{
	string::size_type i;
	string::size_type pos1 = s.find('+');
	string::size_type pos2 = s.find('=');
	string one,two;
	one = s.substr(0,(int)pos1);
	two = s.substr((int)pos1+1,(int)pos2-(int)pos1-1);
	string str;
	map<string,int>::iterator p;
	a = 0;
	for(i=0;i<one.length();i+=3)
	{
		str = one.substr(i,3);
		p = lookup.find(str);
		a = a*10 + p->second;
	}
	b = 0;
	for(i=0;i<two.length();i+=3)
	{
		str = two.substr(i,3);
		p = lookup.find(str);
		b = b*10 + p->second;
	}
}

string transform(int num)
{
	int arr[35] = {0};
	int temp = num;
	string s;
	map<string,int>::iterator p;
	int j = 34;
	while(temp!=0)
	{
		arr[j--] = temp%10;
		temp/=10;
	}
	j=0;
	while((arr[j]==0)&&(j!=35))
		j++;
	if(j==35)
	{
		s = "063";
		return s;
	}
	while(j<35)
	{
		p = lookup.begin();
		while(p->second!=arr[j])
			p++;
		s += p->first;
		j++;
	}
	return s;
}
