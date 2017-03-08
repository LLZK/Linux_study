#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
int makesock(const char* ip,const char* port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("scoket");
		close(sock);
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);

	int on = 1;
	int  retso = setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	if(retso < 0 )
	{
		perror("setsockopt");
		return 5;
	}
	int ret = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
	if(ret < 0)
	{
		perror("bind");
		close(sock);
		return -2;
	}

	int retlis = listen(sock,5);
	if(retlis < 0)
	{
		perror("listen");
		close(sock);
		return -3;
	}

	return sock;
}


int main(int argc,char *argv[])
{
//	printf("hello server\n");

	if(argc < 3)
	{
		printf("Please input : %s [ip] [port]\n",argv[0]);
		return 1;
	}
	int sock = makesock(argv[1],argv[2]);
	struct sockaddr_in socket;
	socklen_t len;
	char buf[1024];
	
	while(1)
	{
		int fd = accept(sock,(struct sockaddr*)&socket,&len);
		if(fd < 0)
		{
			perror("accept");
			//close(sock);
			return 2;
		}
		else if(fd == 0)
		{
			continue;
		}
		while(1)
		{
			//memset(buf,'\0',sizeof(buf));
			int retread = read(fd,buf,sizeof(buf)-1);
			if(retread == 0)
			{
				printf("client is quit !\n");
				break;
			}
			buf[retread] = '\0';
			printf("client#%d   %s\n",retread,buf);
			//fflush(stdout);
		}
		close(fd);
	}
	close(sock);

	return 0;
}
