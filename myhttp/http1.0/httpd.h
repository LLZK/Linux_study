#ifndef __HTTPD_H_
#define __HTTPD_H_


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<assert.h>
#include<ctype.h>
#include<string.h>
#define WARNING 1
#define FATAL 2
#define SIZE 102400

void print_log(const char* str,int error)
{
	printf("%s%s\n",str,error);
}


int startup(const char* ip,const char* port)
{
	assert(ip);
	assert(port);

	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		print_log("socket error---",FATAL);
		exit(1);
	}

	int opt = 1;
	if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0)
	{
		//print_log("setsockopt error---",WARNING);
	}

	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr=inet_addr(ip);
	int len = sizeof(addr);
	if(bind(sock,(struct sockaddr*)&addr,len) < 0)
	{
		print_log("bind error---",FATAL);
		close(sock);
		exit(2);
	}

	if(listen(sock,5) < 0)
	{
		print_log("listen error---",FATAL);
		close(sock);
		exit(3);
	}

	return sock;
}

int get_line(int fd,char* buf,int size)
{
	int i = 0;
	char c;
	while(i < (size -1) && c != '\n')
	{
		int n = recv(fd,&c,1,0);
		if(n > 0)
		{
			if( c == '\r')
			{
				int tmp = recv(fd,&c,1,MSG_PEEK);
				if(tmp > 0 && c == '\n')
				{
					recv(fd,&c,1,0);
				}
				else
				{
					c='\n';
				}
			}
			buf[i++] = c;
		}
		else
		{
			c = '\n';
		}
	}
	buf[i] = '\0';
	return i;
}

void Dealwithsock(int sock)
{
	char buf[SIZE];
	char method[100];
	char url[1024];
	char path[522];
	char Deal[100];

	int cgi = 0;
	
	int s = get_line(sock,buf,sizeof(buf));
    int i = 0;
	int j = 0;

	printf("%s\n",buf);

	while(!isspace(buf[i]) && i < (sizeof(method) - 1))
	{
		method[i] = buf[i];
		i++;
	}
	method[i] = '\0';

	while(isspace(buf[i]) && i < sizeof(buf))
	{
		i++;
	}
	while(!isspace(buf[i]) && j < (sizeof(url) - 1))
	{
		url[j] = buf[i];
		i++;
		j++;
	}
	url[j] = '\0';


	if(strcasecmp("GET",method) && strcasecmp("POST",method))
	{
		print_log("Method is error---",FATAL);
		close(sock);
		exit(3);
	}
	if(!strcasecmp("POST",method))
	{
		cgi = 1;
	}

	if(!strcasecmp("GET",method))
	{
		j = 0;
		while(url[j] != '?' && j < sizeof(path)-1)
		{
			path[j] = url[j];
			j++;
		}
		path[j] = '\0';

		if(url[j] == '?')
		{
			cgi = 1;
		}

	}
	
	while(isspace(buf[i]) && i < sizeof(buf))
	{
		i++;
	}

	int k = 0;
	while(!isspace(buf[i]) && k < sizeof(Deal)-1)
	{
		Deal[k] = buf[i];
		k++;
		i++;
	}

	printf("CGI: %d\n",cgi);
	printf("Method: %s\n",method);
	printf("url: %s\n",url);
	printf("Deal: %s\n",Deal);


}



#endif // __HTTPD_H_
