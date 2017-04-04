#!/bin/bash
SRC=$(echo *.c)
OBJ=$(echo ${SRC} | sed 's/\.c/\.o/g')
CC=gcc
OPTION=lpthread
SERVER_NAME=httpd
echo ${OBJ}
cat << EOF >Makefile
${SERVER_NAME}:${OBJ}
	${CC} -o \$@ \$^ -${OPTION}
%.o:%.c
	${CC} -c \$<
.PHONY:clean
clean:
	rm -rf ${SERVER_NAME} ${OBJ}
EOF
