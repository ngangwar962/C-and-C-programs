//program to generate strings in the form like firstly all of 1 length then 2 length then 3 length and so on....
#include<bits/stdc++.h>
using namespace std;
int main()
{
	int i,j,k;
	string str;
	cin>>str;
	int len=str.length();
	for(i=1;i<=len;i++)
	{
		for(j=0;j+i<=len;j++)
		{
			cout<<str.substr(j,i)<<"\n";	//substring function to generate strings
		}
	}
	return 0;
}
