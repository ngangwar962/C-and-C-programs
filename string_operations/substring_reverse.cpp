#include<iostream>
#include<string.h>
#include<bits/stdc++.h>
using namespace std;
int main()
{
	int i,k,l,j;
	string str;
	cin>>str;
	int len=str.length();
	int flag=1;
	for(i=0;i<len;i++)
	{
		for(j=1;j+i<=len;j++)
		{
			string st=str.substr(i,j);
			reverse(st.begin(),st.end());
			if(str.find(st)==-1)
			{
				cout<<"No"<<"\n";
				flag=1;
				return 0;
			}
		}	
	}
	if(flag==0)
	{
		cout<<"Yes"<<"\n";
		return 0;
	}
	return 0;
}
