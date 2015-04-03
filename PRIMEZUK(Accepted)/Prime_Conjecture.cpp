//Segmentation Fault: Core Dumped.  Th Worst Kind of Error.
//print m if m is prime, else print its largest prime factor.
#include <stdio.h>
#include <math.h>
#include <vector>
#define root 47000
using namespace std;

vector<int> primes;
vector<int> factors;

void prime_sieve();
bool primality_test(int );
void factorize(int );

int main(void)
{
     int i,t,n,j;
     int p,m;
	scanf("%d",&t);
     prime_sieve();
     for(i=0;i<t;i++)
     {
	vector<int> primes_entered;
	factors.clear();	m = 1;
	scanf("%d",&n);
	for(j=0;j<n;j++)
        { 
	  scanf("%d",&p);
          primes_entered.push_back(p);
	}
//	printf("All primes entered\n");
	for(j=0;j<primes_entered.size();j++)
	  m*=primes_entered[j];
	m+=1;
	if(primality_test(m))
	  printf("Case #%d: %d\n",i+1,m);
	else{
	   factorize(m);
	   printf("Case #%d: %d\n",i+1,factors[factors.size()-1]);
	}
     }
     return 0;
}

void factorize(int m)		//called when the number is not prime
{
    int i;
     for(i=0;(i<primes.size())&&(primes[i]<=m/2);i++)
     {
	if(m%primes[i]==0)
	   factors.push_back(primes[i]);
     }
// all prime factors less than 47 k are available.
//now find prime factos more than 47 k
     for(i=0;i<factors.size();i++)
     {
	while(m%factors[i]==0)
	   m/=factors[i];
     }
      if(m!=1)
	factors.push_back(m);
}

//Sieve of Erathosthenes.
void prime_sieve()		//find primes under sqrt(2.2*pow(10,9))
{
    int i,j;
    vector<bool> sieve(66000,false);
    sieve[0] = true;
    sieve[1] = true;
	for(i=2;i<sieve.size();i++)
	{
	    if(sieve[i]==false)
	    {
		primes.push_back(i);
		for(j=i*i;j<sieve.size();j+=i)
	           sieve[j]=true;	
	    }
	}
}
//Primality Test.
bool primality_test(int num)
{
     int i;
     for(i=0;primes[i]<=(int)sqrt(num);i++)
     {
	if(num%primes[i]==0)
	   return false;
     }
	return true;
}
