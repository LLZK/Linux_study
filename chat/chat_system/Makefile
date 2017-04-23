ROOT_PATH=$(shell pwd)
CLIENT_PATH=$(ROOT_PATH)/client
SERVER_PATH=$(ROOT_PATH)/server
COMM_PATH=$(ROOT_PATH)/comm
CONF_PATH=$(ROOT_PATH)/conf
LOG_PATH=$(ROOT_PATH)/log
WINDOW_PATH=$(ROOT_PATH)/window
DATA_POOL_PATH=${ROOT_PATH}/data_pool
CC=g++
SERVER_BIN=udpServer
CLIENT_BIN=udpClient

SERVER_SRC=$(shell ls ${SERVER_PATH} | egrep '^.*\.cpp' )
CLIENT_SRC=$(shell ls ${CLIENT_PATH} | egrep '^.*\.cpp' )

SERVER_OBJ=$(shell echo ${SERVER_SRC} | sed 's/.cpp/.o/g')
CLIENT_OBJ=$(shell echo ${CLIENT_SRC} | sed 's/.cpp/.o/g')

.PHONY:all
all:${SERVER_BIN} ${CLIENT_BIN}

${SERVER_BIN}:${SERVER_OBJ}
	${CC} -o $@ $^
%.o:${SERVER_PATH}/%.cpp
	${CC} -c $<

${CLIENT_BIN}:${CLIENT_OBJ}
	${CC} -o $@ $^
%.o:${CLIENT_PATH}/%.cpp
	${CC} -c $<

.PHONY:clean
clean:
	rm -f ${SERVER_BIN} ${CLIENT_BIN} *.o

.PHONY:debug
debug:
	@echo ${SERVER_SRC}
	@echo ${CLIENT_SRC}
	@echo ${CLIENT_OBJ}
	@echo ${SERVER_OBJ}

