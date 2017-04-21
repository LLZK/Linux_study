#include"sql_cgi.h"

int main()
{
	sqlcon s;
	s.sql_connect();
	s.sql_select();
	return 0;
}
