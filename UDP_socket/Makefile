
.PHONY:all
all:client server

cient:cient.c
	gcc -o $@ $^
server:server.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f client server
