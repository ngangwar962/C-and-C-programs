#include<iostream>
#include<bits/stdc++.h>
#include<string.h>
using namespace std;
int main()
{
	int i,j,k,l;
	string str[]={"hello","are","you","dear"};
	sort(str,str+4);
	for(i=0;i<4;i++)
	cout<<str[i]<<"\n";
	return 0;
}
