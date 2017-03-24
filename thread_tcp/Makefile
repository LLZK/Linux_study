lib=lpthread

.PHONY:all
all:client server

client:client.c
	gcc -o $@ $^ -$(lib)
server:server.c
	gcc -o $@ $^ -$(lib)

.PHONY:clean
	rm -f client server
