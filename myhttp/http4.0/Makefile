.PHONY:all
all:httpd cgi

httpd:httpd.o main.o
	gcc -o $@ $^ -lpthread
%.o:%.c
	gcc -c $<

.PHONY:cgi
cgi:
	cd cgi;make;make output;cd -

.PHONY:clean
clean:
	rm -rf httpd httpd.o main.o cgifun output;cd cgi;make clean;cd -

.PHONY:output
output:all
	mkdir output
	cp httpd output/
	cp -rf log output/
	cp -rf conf output/
	cp -rf wwwroot output/
	cp http_ctl.sh output/
