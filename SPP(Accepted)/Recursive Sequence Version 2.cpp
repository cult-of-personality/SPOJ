/*	To find: S(n)-S(m-1)mod p	*/
/*	Tricky Cases: 	m = 0, m=n-1, */
#include <iostream>
#include <vector>
using namespace std;

void func();
void multiply(vector<vector<long long int> >&,vector<vector<long long int> >&);
void multiply(vector<vector<long long int> >&);
long long int exponent(long long int );
void toBinary(long long int ,int binary[]);

vector<int> b,c;
long long int m,n,p;
int k;
long long int sum_k;

int main() {
	int t;
	cin>>t;
	for(int i=0;i<t;i++){
		b.clear();	c.clear();
		cin>>k;
		b.resize(k);	c.resize(k);
		for(int j=0;j<k;j++)
			cin>>b[j];
		for(int j=0;j<k;j++)
			cin>>c[j];
		cin>>m>>n>>p;
		func();
	}
	return 0;
}

void func(){
	sum_k=0;
	for(int i=0;i<k;i++)
		sum_k = (sum_k%p + b[i]%p)%p;
	long long int res1,res2;
	res1 = exponent(m-1);
	res2 = exponent(n);
	if(res2<res1)
		cout<<(res2-res1+p)<<endl;
	else cout<<(res2-res1)<<endl;
}

long long int exponent(long long int x){
	/*	return S(x)	*/
	if(x<=k){
		long long int sum = 0;
		for(int i=0;i<x;i++)
			sum = (sum%p + b[i]%p)%p;
		return sum;
	}
	int binary[65]={0};
	toBinary(x-k,binary);		/*Verification required	*/
	int i=0;
	while(binary[i]==0)
		i++;
	/*Build coefficient matrix	*/
	vector<vector<long long int> > coeff(k+1,vector<long long int>(k+1));
	coeff[0][0] = 1;	coeff[1][0] = 0;
	for(int j=1;j<=k;j++){
		coeff[0][j] = c[j-1];
		coeff[1][j] = c[j-1];
	}
	for(int q=2;q<=k;q++)
	{
		for(int r=0;r<=k;r++){
			coeff[q][r] = (q==r+1)?1:0;
		}
	}
	/*	Print coefficient matrix	*/
	vector<vector<long long int> > arr(k+1,vector<long long int>(k+1));
	for(int q=0;q<=k;q++){
		for(int r=0;r<=k;r++)
			arr[q][r] = (q==r)?1:0;
	}
	for(;i<65;i++){
		multiply(arr);
		if(binary[i]==1)
			multiply(arr,coeff);
	}
	vector<long long int> arr2(k+1);
	arr2[0] = sum_k;
	for(int j=1;j<=k;j++)
		arr2[j] = b[k-j];
	long long int res = 0;
	for(int j=0;j<=k;j++)
		res = (res%p + ((arr[0][j]%p)*(arr2[j]%p))%p)%p;
	return res;
}

void toBinary(long long int x,int bin[]){
	int j = 64;
	while(x!=0){
		bin[j] = x%2;
		x/=2;
		j--;
	}
}

void multiply(vector<vector<long long int> > &v){
	vector<vector<long long int> > res(k+1,vector<long long int>(k+1,0));
	for(int i=0;i<=k;i++){
		for(int j=0;j<=k;j++){
			for(int l=0;l<=k;l++){
				res[i][j] = (res[i][j]%p + ((v[i][l]%p)*(v[l][j]%p))%p)%p;
			}
		}
	}
	for(int i=0;i<=k;i++){
		for(int j=0;j<=k;j++)
			v[i][j] = res[i][j];
	}
}

void multiply(vector<vector<long long int> > &v1,vector<vector<long long int> > &v2){
	vector<vector<long long int> > res(k+1,vector<long long int>(k+1,0));
	for(int i=0;i<=k;i++){
		for(int j=0;j<=k;j++){
			for(int l=0;l<=k;l++)
				res[i][j] = (res[i][j]%p + ((v1[i][l]%p)*(v2[l][j]%p))%p)%p;
		}
	}
	for(int i=0;i<=k;i++){
		for(int j=0;j<=k;j++)
			v1[i][j] = res[i][j];
	}
}