//checking if the string is palindrome or not by counting the character
#include<bits/stdc++.h>
using namespace std;
int main()
{
	int i,j,k,l;
	string str;
	cin>>str;
	int len=str.length();
	int even_count=0,odd_count=0;
	int count[26]={0};	//count array
	i=0;
	while(str[i]!='\0')
	{
		cout<<str[i]-'a'<<"\n";
		count[str[i]-'a']++;
		i++;
	}
	for(i=0;i<26;i++)
	{
		if(count[i])	
		{
			if(count[i]%2)		//counting odd count
			odd_count++;	
			else			//counting even count
			even_count++;
		}		
	}
	cout<<even_count<<" "<<odd_count<<"\n";		
	if(odd_count==1&&len!=1)		//if odd count is only 1
	{
		cout<<"YES"<<endl;
		return 0;
	}
	if(even_count>0&&odd_count==0)		//if odd count is 0 and even count is greater than 0
	{
		cout<<"YES"<<endl;
		return 0;
	}
	if(odd_count>1)			// if odd count is greater than 1 than not palindrome
	{
		cout<<"NO"<<endl;
		return 0;
	}
}
