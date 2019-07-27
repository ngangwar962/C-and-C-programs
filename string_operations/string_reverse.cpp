//program to reverse string using reverse function and by brute force also
#include<bits/stdc++.h>
using namespace std;
int main()
{
	int i,j,k,start,last;
	string str;
	cin>>str;
	int len=str.length();//program to calculate the string length
	reverse(str.begin(),str.end());	//directly using reverse function to reverse the string
	cout<<str<<"\n";	//string after reversing the original string
	start=0;last=len-1;		//will reverse the reverse string to give original string
	while(start<last)
	{
		char temp;
		temp=str[start];
		str[start]=str[last];
		str[last]=temp;
		start++;
		last--;
	}
	cout<<str<<"\n";	//original string
	return 0;
}
