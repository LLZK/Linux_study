#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<pthread.h>

int makesock(const char* ip,const char* port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(0);
	}

	int opt = 1;
	int retset = setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));


	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);

	int retbind = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
	if(retbind < 0)
	{
		perror("bind");
		exit(1);
	}

	int retlis = listen(sock,5);
	if(retlis < 0)
	{
		perror("listen");
		exit(2);
	}
	return sock;
}

void* thread_sev(void *arg)
{
//	pthread_detach(pthread_self());
	int sock = *(int *)arg;
	char buf[1024];
	int len = sizeof(buf);
	while(1)
	{
        ssize_t size = read(sock,buf,len);
		if(size > 0)
		{
			buf[size -1] = '\0';
			printf("client # %s\n",buf);
			memset(buf,'\0',len);
		}
		else if(size == 0)
		{
			printf("client is quit!\n");
			break;
		}
		else
		{
			perror("read");
			break;
		}
	}
	close(sock);
	
}



int main(int argc,char*argv[])
{

	if(argc < 3)
	{
		printf("Please input:%s [ip] [port]\n",argv[0]);
		exit(-1);
	}

	int sock = makesock(argv[1],argv[2]);

	struct sockaddr_in peer;
	int len = 0;

	while(1)
	{
		//printf("Debug\n");
		int ret = accept(sock,(struct sockaddr*)&peer,&len);
		//printf("Debug ret %d\n",ret);
		if(ret < 0)
		{
			continue;
		}
		printf("Get a client# IP:%s PORT:%d\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port));
		pthread_t id;
		pthread_create(&id,NULL,thread_sev,&ret);
		pthread_join(id,NULL);
	}
	
	//printf("hello server\n");
	return 0;
}
