#!/bin/bash
ROOT_PATH=$(pwd)


# httpd src
SRC=$(echo *.c)
OBJ=$(echo ${SRC} | sed 's/\.c/\.o/g')
CC=gcc
OPTION=lpthread
SERVER_NAME=httpd

# cgi src
CGI_PATH=${ROOT_PATH}/cgi
CGI_SRC=$(ls $CGI_PATH | grep "cgi" | grep '.c$')
CGI_BIN=cgifun

cat << EOF > $CGI_PATH/Makefile
${CGI_BIN}:$CGI_SRC
	$CC -o \$@ \$^

.PHONY:clean
clean:
	rm -f ${CGI_BIN}
.PHONY:output
output:
	cp $CGI_BIN ..
EOF


cat << EOF >Makefile
.PHONY:all
all:${SERVER_NAME} cgi

${SERVER_NAME}:${OBJ}
	${CC} -o \$@ \$^ -${OPTION}
%.o:%.c
	${CC} -c \$<

.PHONY:cgi
cgi:
	cd cgi;make;make output;cd -

.PHONY:clean
clean:
	rm -rf ${SERVER_NAME} ${OBJ} ${CGI_BIN} output;cd cgi;make clean;cd -

.PHONY:output
output:all
	mkdir output
	cp ${SERVER_NAME} output/
	cp -rf log output/
	cp -rf conf output/
	cp -rf wwwroot output/
	cp http_ctl.sh output/
EOF
