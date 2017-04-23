#ifndef __HTTPD_H_
#define __HTTPD_H_


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/sendfile.h>
#include<syslog.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<assert.h>
#include<ctype.h>
#include<string.h>
#include<fcntl.h>

#define WARNING 0
#define FATAL 1
#define SIZE 102400

void print_log(const char* str,int error,int lines);
void send_error(int sock,const char* path);
void echo_error(int sock,int error_code);
int startup(const char* ip,const char* port);
int get_line(int fd,char* buf,int size);
void clear_header(int sock);
int echo_cgi(int sock,const char* method,const char* path,const char* query);
void Dealwithsock(int sock);

#endif // __HTTPD_H_
