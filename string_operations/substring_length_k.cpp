#include<iostream>
using namespace std;
int palindrome(string str,int start,int last)
{
	if(start>=last)
	return 1;
	if(str[start]!=str[last])
	{
		return 0;
	}
	
	return palindrome(str,start+1,last-1);
}
int main()
{
	int i,j,k;
	string str;
	cout<<"enter the string"<<"\n";
	cin>>str;
	cout<<"enter the value of k also"<<"\n";
	cin>>k;
	int len=str.length();
	int count=0;
	cout<<"only palindromic substrings are"<<"\n";
	for(i=0;i<len;i++)
	{
		if(i+k<=len)
		{
			string st=str.substr(i,k);
			if(palindrome(st,0,k-1))
			{
				cout<<st<<"\n";	
				count++;
			}
		}
	}
	cout<<"count of palindromic strings are "<<count<<"\n";
	return 0;
}
