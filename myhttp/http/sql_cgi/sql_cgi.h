#include<iostream>
#include<mysql.h>
#include<string>
#include<cstring>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

class sqlcon
{
	public:
		sqlcon()
		{
			if((sql= mysql_init(NULL)) == NULL)
			{
				cout<<"init error"<<endl;
			}
		}
		bool sql_connect()
		{
			bool ret = true;;
			if(mysql_real_connect(sql,"localhost","root",NULL,"living",0,NULL,0) == NULL)
			{
				perror("connect");
				ret = false;
			}
			return ret;
		}
		bool sql_insert(const string str)
		{
		    bool ret = true;
			string insert_str = "INSERT INTO tbl_douyu(name,platform,room_id,id)values(";
			insert_str += str;
			insert_str += ")";
			if(mysql_query(sql,insert_str.c_str()) != 0)
			{
				ret = false;
			}
			return ret;
		}

		bool sql_select()
		{
			bool ret = true;
			string str = "select * from tbl_douyu";
			if(mysql_query(sql,str.c_str()) != 0)
			{
				ret = false;
			}
			MYSQL_RES *result;
			MYSQL_ROW row;
			MYSQL_FIELD *field;
			int num;
			result = mysql_store_result(sql);
			num = mysql_num_fields(result);

			int i;

			while(field = mysql_fetch_field(result))
			{
				printf("%s ",field->name);
			}
			while(row = mysql_fetch_row(result))
			{
				for(i = 0;i < num;i++)
				{
					printf("%s ",row[i] ? row[i] : "NULL");
				}
				printf("\n");
			}

			mysql_free_result(result);
			return ret;

		}

		~sqlcon()
		{
			mysql_close(sql);
		}
	private:
		MYSQL *sql;
};
