//program to remove spaces from the strings
#include<iostream>
using namespace std;
int main()
{
	int i,j,k,l;
	string str;
	getline(cin,str);
	cout<<str<<"\n";
	i=0;
	while(str[i]!='\0')
	{
		while(str[i]==' ')
		{
			str.erase(i,1);	//erase function to edit string
		}
		i++;
	}
	cout<<str<<"\n";
	return 0;
}
