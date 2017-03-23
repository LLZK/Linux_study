#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
int makesock(const char* ip,const char* port)
{
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
	{
		perror("socket");
		return -2;
	}
	int opt = 1;
	//setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);

	int ret = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
	if(ret < 0)
	{
		perror("bind");
		return -3;
	}

/*	int retlis = listen(sock,5);
	printf("Debug %d\n",retlis);
	if(retlis < 0)
	{
		perror("listen");
		return -4;
	}
*/
	return sock;
}



int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Please input:%s [ip][port]\n",argv[1]);
		return -1;
	}

	int sock = makesock(argv[1],argv[2]);

	struct sockaddr_in addr;
	int len = 0;
	char buf[1024];
	while(1)
	{
		memset(buf,0,sizeof(buf));
		ssize_t size = recvfrom(sock,buf,sizeof(buf),0,(struct sockaddr*)&addr,&len);
		
		if(size == 0)
		{
			printf("client is quit\n");
			break;
		}
		else if(size < 0)
		{
			perror("recvfrom");
			continue;
		}
		else
		{
			printf("SIZE: %d IP: %s  PORT:%d\n",size,inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
			buf[size-1] = '\0';
			printf("client# %s\n",buf);
		}
	}
	close(sock);
	return 0;
}
