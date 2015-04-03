#include <stdio.h>
#include <math.h>
#include <vector>
using namespace std;

void prime_sieve();
void factorize(int );
bool primality_test(int );
int calc(int);

vector<int> primes;
vector<int> factors;

int main(void)
{
    int i,t,n;
	scanf("%d",&t);
    primes.clear();
    prime_sieve();
    for(i=0;i<t;i++)
     {
	scanf("%d",&n);
	if(n==1)
	{
	   printf("1\n");
	   continue;
	}
         if(primality_test(n))
	    printf("%d\n",n-1);
	 else
	  {
	     factors.clear();
	      factorize(n);
/*	printf("\nFactors are:-\n");
	for(int z=0;z<factors.size();z++)
		printf("%d\t",factors[z]);
*/	      printf("%d\n",calc(n)); 
	  }
     }
    return 0;
}

void prime_sieve()
{
    int i,j;
    vector<bool> sieve(1001,false);
    sieve[0] = true;
    sieve[1] = true;
	for(i=2;i<sieve.size();i++)
	{
	    if(!sieve[i])
	    {
		primes.push_back(i);
		for(j=i*i;j<sieve.size();j+=i)
	           sieve[j]=true;	
	    }
	}
}

bool primality_test(int num)
{
    int i;
    int root = (int)sqrt(num);
     for(i=0;primes[i]<=root;i++)
     {
	if(num%primes[i]==0)
	  return false;
     }
     return true;
}

void factorize(int n)		//called only when n is composite.
{
   int temp = n;
   for(int i=0;i<primes.size();i++)
   {
	if(primes[i]>n)
	   break;
	if(temp%primes[i]==0)
	{
	   factors.push_back(primes[i]);
	   while(temp%primes[i]==0)
		temp/=primes[i];		//removing all powers of primes[i]
	}
   }
    if(temp!=1)
	factors.push_back(temp);
}


int calc(int n)
{
    int prod=1;
    if(factors.empty())
        return n-1;
    for(int i=0;i<factors.size();i++)
      prod*=factors[i];
    n=(n/prod);
    for(int i=0;i<factors.size();i++)
      n*=(factors[i]-1);
    return n;
}
