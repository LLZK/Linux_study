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

void print_log(const char* str,int error)
{
	char *err[2]= {"WARNING","FATAL"};
	openlog("print_log",LOG_CONS|LOG_PID,LOG_USER);

	syslog(LOG_INFO,"%s%s\n",str,err[error]);
	closelog();
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

void clear_header(int sock)
{
	char buf[1024];
	int ret = 0;
	do{
		ret = get_line(sock,buf,sizeof(buf));
	}while(ret != 1 && strcmp("\n",buf));
}

int echo_www(int sock,const char* path,int size)
{
	int outfd = open(path,O_RDONLY);
	if(outfd < 0)
	{
		print_log("OPEN file(get out_fd) is error---",FATAL);
		return -1;
	}
	
	char buf[200] = "HTTP/1.0 200 OK\r\n\r\n<html></html>";
	int headret = send(sock,buf,sizeof(buf),0);
	if(headret <= 0)
	{
		print_log("ECHO head is error---",FATAL);
		return -1;

	}
	int ret= 0;
	if((ret = sendfile(sock,outfd,0,size)) < 0)
	{
		print_log("ECHO (CGI=0) is error---",FATAL);
		return -1;
	}
	return ret;
}

int echo_cgi(int sock,const char* method,const char* path,const char* query)
{
	char buf[1024] = {0};
	int len = 0;
	if(strcmp("GET",method) != 0)
	{ //POST
		printf("POST\n");
		int ret = 0;
		do{
//			buf[15]='\0';
			ret = get_line(sock,buf,sizeof(buf));
			//printf("ret = %d\n",ret);
			printf("%s\n",buf);
			if(strncmp("Content-Length:",buf,15) == 0){
				//printf("111111\n");
				len = atoi(&(buf[16]));
				//printf("%s\n",&buf[16]);
			}
			//break? or no break?
		}while(ret > 0 && *buf != '\n');
		printf("len is %d\n",len);
		if(len <= 0){
			print_log("Content-Length is error---",FATAL);
			exit(-1);
		}
	}
	else
	{//GET
		clear_header(sock);
	}
	int input[2];
	int output[2];

	if(pipe(input)< 0 || pipe(output) < 0){
		print_log("pipe create error---",FATAL);
		close(sock);
		exit(-1);
	}
	printf("pipe is successful\n");
	pid_t id = 0;
	id = fork();
	printf("id = %d\n",id);
	if(id < 0){
		print_log("fork child error---",FATAL);
		close(sock);
		exit(-1);
	}
	else if(id > 0){
		close(input[0]);
		close(output[1]);

		if(strcasecmp("POST",method) == 0){
			char ch = '\0';
			int i = 0;
			for(i = 0;i < len;i++)
			{
				recv(sock,&ch,1,0);
				write(input[1],&ch,1);
			}
		}

		// get result
		char ch = '\0';
        while(read(output[0],&ch,1) > 0)
		{
			send(sock,&ch,1,0);
		}
		close(sock);
		close(input[1]);
		close(output[0]);

		waitpid(id,NULL,0);
	}
	else{
		printf("id ======= 0\n");
		close(input[1]);
		close(output[0]);

		dup2(input[0],0);
		dup2(output[1],1);


		//printf("child method is %s\n",method);
		char Method[255] = {0};
		char content_len[255] = {0};
		char query_string[255] = {0};

		sprintf(Method,"METHOD=%s",method);
		putenv(Method);
		if(!strcasecmp(method,"POST"))
		{
			sprintf(content_len,"CONTENT_LEN=%s",len);
			putenv(content_len);
		}
		//printf("child method is %s\n",method);
		if(!strcasecmp(method,"GET"))
		{
			sprintf(query_string,"QUERY_STRING=%s",query);
			putenv(query_string);
		}
		//printf("child method is %s\n",method);
		//char Path[1024] = {0};
		//printf("child method is %s\n",method);
		execl(path,path,NULL);
		printf("execl is error\n");
		exit(0);
	}
}

void Dealwithsock(int sock)
{
	char buf[SIZE];
	char method[100];
	char url[1024];
	char path[522];
	char Deal[100];
	char *query_string = NULL;

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
			query_string=url+j+1; // get "GET" arguments
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

	char All_path[522]={0};
	sprintf(All_path,"wwwroot%s",path);

	printf("%s-------------ALL_PATH\n",All_path);

	if(All_path[strlen(All_path)-1] == '/')
	{
		printf(" '\\'  exist...\n ");
		strcat(All_path,"index.html");
		printf("AllPath:%s\n",All_path);
	}
	struct stat st;

	if(stat(All_path,&st) < 0)
	{
		clear_header(sock);
		print_log("FILE is unknow---",FATAL);
		exit(4);
	}
	else
	{
	    printf("'\\' no  exist...\n");
		if(st.st_mode & S_IFDIR)
		{
			printf("file is dir...\n");
			All_path[strlen(All_path)] = '/';
			strcat(All_path,"index.html");
			printf("AllPath:%s\n",All_path);
		}
	}
	
	printf("CGI: %d\n",cgi);

	if((st.st_mode & S_IXUSR) ||\
	   (st.st_mode & S_IXGRP) ||\
	   (st.st_mode & S_IXOTH))
	{
		cgi = 1;
	}
	printf("CGI: %d\n",cgi);

	if(cgi == 1)
	{
		echo_cgi(sock,method,All_path,query_string);
	}
	else
	{
		printf("cgi ======== 0\n");
		clear_header(sock);

		printf("cgi ======== 0\n");
		int ret = echo_www(sock,All_path,st.st_size);
		if(ret < 0){
			print_log("ECHO_WWW is error---",FATAL);
			exit(5);
		}
		close(sock);
	}
}



#endif // __HTTPD_H_
