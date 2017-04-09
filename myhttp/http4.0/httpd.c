#include"httpd.h"

void print_log(const char* str,int error,int lines)
{
	char *err[2]= {"WARNING","FATAL"};
	openlog("print_log",LOG_CONS|LOG_PID,LOG_USER);

	syslog(LOG_INFO,"lines:%d %s %s\n",lines,str,err[error]);
	closelog();
}

void send_error(int sock,const char* path)
{
	if(path == NULL)
	{
		print_log("error page path is NULL---",FATAL,__LINE__);
		return;
	}
	struct stat st;
	if(stat(path,&st) < 0)
	{
		print_log("error page not found---",FATAL,__LINE__);
		return;
	}
	int fd = open(path,O_RDONLY);
	int ret = 0;
	if(ret = sendfile(sock,fd,0,st.st_size) < 0)
	{
		print_log("Error send error_page---",FATAL,__LINE__);
	}
}


void echo_error(int sock,int error_code)
{
	switch(error_code)
	{
	case 401:
		{
		char buf[100] = "HTTP/1.0 401 Unauthorized\r\n\r\n";
		send(sock,buf,strlen(buf),0);
		send_error(sock,"wwwroot/error/401.html");
		break;
		}
	case 400:
		{
		char buf[100] = "HTTP/1.0 400 Bad Request\r\n\r\n";
		send(sock,buf,strlen(buf),0);
		send_error(sock,"wwwroot/error/400.html");
		break;
		}
	case 403:
		{
		char buf[100] = "HTTP/1.0 403 Forbidden\r\n\r\n";
		send(sock,buf,strlen(buf),0);
		send_error(sock,"wwwroot/error/403.html");
		break;
		}
	case 404:
		{
		char buf[100] = "HTTP/1.0 404 Not Found\r\n\r\n";
		send(sock,buf,strlen(buf),0);
		send_error(sock,"wwwroot/error/404.html");
		break;
		}
	case 500:
		{
		char buf[100] = "HTTP/1.0 500 Internal Server Error\r\n\r\n";
		send(sock,buf,strlen(buf),0);
		send_error(sock,"wwwroot/error/500.html");
		break;
		}
	case 503:
		{
		char buf[100] = "HTTP/1.0 503 Server Unavailable\r\n\r\n";
		send(sock,buf,strlen(buf),0);
		send_error(sock,"wwwroot/error/503.html");
		break;
		}
	default:
    	break;
	}
}

int startup(const char* ip,const char* port)
{
	assert(ip);
	assert(port);

	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		print_log("socket error---",FATAL,__LINE__);
		exit(1);
	}

	int opt = 1;
	if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0)
	{
		print_log("setsockopt error---",WARNING,__LINE__);
	}

	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr=inet_addr(ip);
	int len = sizeof(addr);
	if(bind(sock,(struct sockaddr*)&addr,len) < 0)
	{
		print_log("bind error---",FATAL,__LINE__);
		close(sock);
		exit(2);
	}

	if(listen(sock,5) < 0)
	{
		print_log("listen error---",FATAL,__LINE__);
		close(sock);
		exit(3);
	}

	return sock;
}

int get_line(int fd,char* buf,int size)
{
	int i = 0;
	char c = '\0';
	while(i < (size -1) && (c != '\n'))
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
			buf[i] = c;
			i++;
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
	}while(ret > 0 && strcmp("\n",buf));
}

int echo_www(int sock,const char* path,int size)
{
	int outfd = open(path,O_RDONLY);
	if(outfd < 0)
	{
		print_log("OPEN file(get out_fd) is error---",FATAL,__LINE__);
		return -1;
	}
	
	char buf[200] = "HTTP/1.0 200 OK\r\n\r\n";
	int headret = send(sock,buf,strlen(buf),0);
	if(headret <= 0)
	{
		echo_error(sock,500);
		print_log("ECHO head is error---",FATAL,__LINE__);
		return -1;

	}
	int ret= 0;
	if((ret = sendfile(sock,outfd,0,size)) < 0)
	{
		echo_error(sock,500);
		print_log("ECHO (CGI=0) is error---",FATAL,__LINE__);
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
		int ret = 0;
		do{
			ret = get_line(sock,buf,sizeof(buf));
			if(strncmp("Content-Length: ",buf,16) == 0){
				len = atoi(&(buf[16]));
			}
		}while(ret > 1 && strcmp("\n",buf) != 0);
	
		if(len <= 0){
			echo_error(sock,500);
			print_log("Content-Length is error---",FATAL,__LINE__);
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
		echo_error(sock,500);
		print_log("pipe create error---",FATAL,__LINE__);
		close(sock);
		exit(-1);
	}
	pid_t id = 0;
	id = fork();
	if(id < 0){
		echo_error(sock,500);
		print_log("fork child error---",FATAL,__LINE__);
		close(sock);
		exit(-1);
	}
	else if(id == 0){
		close(input[1]);
		close(output[0]);

		char Method[255] = {0};
		char content_len[255] = {0};
		char query_string[255] = {0};

		sprintf(Method,"METHOD=%s",method);
		putenv(Method);
		if(!strcasecmp(method,"POST"))
		{
			sprintf(content_len,"CONTENT_LEN=%d",len);
			putenv(content_len);
		}
		if(!strcasecmp(method,"GET"))
		{
			sprintf(query_string,"QUERY_STRING=%s",query);
			putenv(query_string);
		}
	   char buf[512] = {0};
	   strcpy(buf,"HTTP/1.0 200 OK\r\n");
	   send(sock,buf,strlen(buf),0);


	   send(sock,"\r\n",strlen("\r\n"),0);
	  
		dup2(input[0],0);
		dup2(output[1],1);
		execl(path,path,NULL);
		exit(0);
	}
	else{
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
		close(input[1]);
		close(output[0]);
		close(sock);

		waitpid(id,NULL,0);
		return 0;
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
		echo_error(sock,400);
		print_log("Method is error---",FATAL,__LINE__);
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

	char All_path[522]={0};
	if(!strcasecmp("GET",method))
	{
		sprintf(All_path,"wwwroot%s",path);
	}
	else if(!strcasecmp("POST",method))
	{
		sprintf(All_path,"wwwroot%s",url);
	}



	if(All_path[strlen(All_path)-1] == '/')
	{
		strcat(All_path,"index.html");
	}
	struct stat st;

	if(stat(All_path,&st) < 0)
	{
		clear_header(sock);
		echo_error(sock,404);
		print_log("FILE is unknow---",FATAL,__LINE__);
		exit(4);
	}
	else
	{
		if(st.st_mode & S_IFDIR)
		{
			All_path[strlen(All_path)] = '/';
			strcat(All_path,"index.html");
		}
	}
	

	if((st.st_mode & S_IXUSR) ||\
	   (st.st_mode & S_IXGRP) ||\
	   (st.st_mode & S_IXOTH))
	{
		cgi = 1;
	}

	if(cgi == 1)
	{
		echo_cgi(sock,method,All_path,query_string);
	}
	else
	{
		clear_header(sock);

		int ret = echo_www(sock,All_path,st.st_size);
		if(ret < 0){
			print_log("ECHO_WWW is error---",FATAL,__LINE__);
			exit(5);
		}
		close(sock);
	}
}



