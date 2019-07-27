#include<iostream>
#include<bits/stdc++.h>
#include<string.h>
using namespace std;
int main()
{
	int i,j,k,l;
	string s1;
	cin>>s1;
	int len=s1.length();
	int flag=0;
	for(i=0;i<len;i++)
	{
		for(j=1;j<=len-i;j++)
		{
			string str(s1.substr(i,j));
			cout<<str<<"\n";
			reverse(str.begin(),str.end());
			if((s1.find(str))==-1)
			{
				flag=1;
				cout<<"not a palindrome"<<"\n";
				return 0;
			}
		}
	}
	if(flag==0)
	{
		cout<<"reverse for each and every string exists"<<"\n";
		return 0;
	}
	return 0;
}
