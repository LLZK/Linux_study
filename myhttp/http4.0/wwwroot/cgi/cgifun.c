#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
int main()
{

	printf("<html><br>\n");
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
		strcpy(data,query);
	}
	if(strcmp(method,"POST") == 0){

		char ch = '\0';
		int sz = 0;
		int i = 0;
		sz = read(0,data,sizeof(data));

	}
	printf("<img src='su8.jpg'>\n");
	printf("<h1>hello world</h1><br/>\n");
	printf("</html>");
	return 0;
}
