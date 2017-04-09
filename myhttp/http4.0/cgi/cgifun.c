#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
	printf("<html>\n");
	printf("<h1>nihao</h1></html>\n");
	
	char *method = getenv("METHOD");
	printf("%s\n",method);
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
		strcpy(data,query);
		printf("%s\n",query);
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
	printf("<html>hello world<html>\n");
	return 0;
}
