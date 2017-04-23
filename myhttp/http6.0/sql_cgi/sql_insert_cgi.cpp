#include"sql_cgi.h"

string get_data()
{
	char *method = getenv("METHOD");
	string str;
	if(method == NULL)
	{
		printf("method is error\n");
		return str;
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
	return str;
}


int main()
{
	string str = get_data();
	sqlcon s;
	if(s.sql_connect() == false)
	{
		cout<<"error connect"<<endl;
		return -1;
	}
	if(!str.empty())
	{
		if(s.sql_insert(str) == false)
		{
			cout<<"error insert"<<endl;
			return -1;
		}
	}
	return 0;
}

