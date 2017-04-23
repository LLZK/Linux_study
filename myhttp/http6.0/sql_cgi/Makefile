ROOT_PATH=$(shell pwd)
INCLUDE_PATH=-I$(ROOT_PATH)/../include
LIB_PATH=-L$(ROOT_PATH)/../lib
LIB=-lmysqlclient -lpthread

.PHONY:all
all:sql_insert_cgi sql_select_cgi

sql_insert_cgi:sql_insert_cgi.cpp
	g++ -o $@ $^ ${INCLUDE_PATH} ${LIB_PATH} ${LIB}
sql_select_cgi:sql_select_cgi.cpp
	g++ -o $@ $^ ${INCLUDE_PATH} ${LIB_PATH} ${LIB}
	

.PHONY:clean
clean:
	rm -f sql_insert_cgi
