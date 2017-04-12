#include<stdio.h>
#include<stdlib.h>
#include<cstring>
#include<unistd.h>
#include<iostream>
using namespace std;
void get_data()
{
	char *method = getenv("METHOD");
	string str;
	if(method == NULL)
	{
		printf("method is error\n");
		return;
	}
	int i = 0;
	if(strcmp("GET",method) == 0)
	{
		char *query = getenv("QUERY_STRING");
		while(*query != '\0')
		{
			if(*query == '=')
			{
				query++;
				str+="\'";
				while(*query != '&' && *query != '\0')
				{
					str+= *query;
					query++;
				}
				if(*query == '&')
				{
					str+="\'";
					str+=",";
				}
				else
				{
					str+="\'";
					break;
				}
			};
			query++;
		}
	}
	cout<<str<<endl;
}
int main()
{
	get_data();
	return 0;
}
