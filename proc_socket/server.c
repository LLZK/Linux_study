#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

int makesock(const char* ip,const char* port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		printf("socket");
		return -1;
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
		return -2;
	}

	int retlis = listen(sock,5);
	if(retlis < 0)
	{
		perror("listen");
		return -3;
	}

	return sock;
}


int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Please input:%s [ip] [port]\n",argv[0]);
		return -1;
	}

	int sock = makesock(argv[1],argv[2]);

	struct sockaddr_in addr;
	int len = 0;
	char buf[1024];

	while(1)
	{
		int ret = accept(sock,(struct sockaddr*)&addr,&len);

		pid_t id = fork();
		if(id < 0)
		{
			perror("fork");
			return -2;
		}
		else if(id == 0)
		{
			if(fork() > 0)
			{
				exit(1);
			}
			else
			{
				while(1)
				{
					int size = read(ret,buf,sizeof(buf));
					if(size == 0)
					{
						close(ret);
						printf("client is quit!\n");
						break;
					}
					//printf("Debug : %s\n",buf);
					buf[size-1] = '\0';
					printf("SIZE:%d IP:%s PORT:%d\n",size,inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
					printf("Client# %s\n",buf);
				}
				close(ret);
			}
		}
		else
		{
			int status;
			//while(waitpid(-1,&status,WNOHANG) > 0);
			waitpid(-1,&status,0);
		}
	}
	close(sock);
	return 0;
}
