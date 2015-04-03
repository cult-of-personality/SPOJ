#include <iostream>
#include <vector>
using namespace std;

struct object
{
   int space;
   int value;
};

class state
{
  public:
    vector<bool> used;
    int value;
    state()
    {
	   value = 0;
    }
};

int main()
{
   int n,s,max,new_value,new_state;
   cin>>s>>n;
   vector<state> tot_val(s+1);
   vector<object> obj(n);
	(tot_val[0].used).resize(n);
    for(int i=0;i<n;i++)				/*	gathers info about objects to be used from user	*/
		cin>>obj[i].space>>obj[i].value;
	for(int i=0;i<tot_val.size();i++)
	{
		for(int j = 0;j<tot_val[i].used.size();j++)
		{
		  if(!(tot_val[i].used[j]))		/*  if the particular object has not been used yet  */
		  {
			new_value = tot_val[i].value + obj[j].value;
			new_state = i + obj[j].space;
			if(new_state<tot_val.size())
			{
			  if(new_value>tot_val[new_state].value)
			  {
				tot_val[new_state].used = tot_val[i].used;
				tot_val[new_state].used[j] = true;
				tot_val[new_state].value = new_value;
			  }
			}
		  }
		}
	}
    max = tot_val[tot_val.size()-1].value;
    for(int i=tot_val.size()-1;i>=0;i--)
    {
		if(tot_val[i].value>max)
	  	max = tot_val[i].value;
    }
    cout<<max<<endl;
/*   for(int i=0;i<tot_val.size();i++)
      cout<<"Tot_Val["<<i<<"] = "<<tot_val[i].value<<endl;
*/    return 0;
}
