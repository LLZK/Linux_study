#include<stdio.h>
#include<stdlib.h>
int main()
{
	char *method = getenv("METHOD");
	char first[255] = {0};
	char last[255] = {0};
	char data[512] = {0};
	if(method == NULL)
	{
		printf("method is error\n");
		return -1;
	}
	int i = 0;
	if(strcmp("GET",method) == 0)
	{
		char *query = getenv("QUERY_STRING");
		while(*query != '&')
		{
			if(*query == '=')
			{
				query++;
				first[i] = *query;
				i++;
			}
			query++;
		}
		query++;
		while(*query != '\0')
		{
			if(*query == '=')
			{
				query++;
				last[i] = *query;
				i++;
			}
			query++;
		}
	}
	if(strcmp(method,"POST") == 0){

		char ch = '\0';
		int sz = 0;
		sz = recv(0,&ch,1,0);
		while(sz > 0)
		{
			if(ch == '=')
			{

			}
		}

	}
	printf("<html>hello world<html>");
	return 0;
}
