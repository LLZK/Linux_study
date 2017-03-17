#define _GNU_SOURCE


#include<stdio.h>
#include<poll.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<fcntl.h>
int main(int argc,char*argv[])
{
	if(argc < 3)
	{
		printf("Please input# %s [ip] [port]\n",argv[0]);
		return -1;
	}
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		return -2;
	}



     struct	pollfd fd[2];

	fd[0].fd = 0;
	fd[0].events = POLLIN;
	fd[0].revents = 0;

	fd[1].fd = sock;
	fd[1].events = POLLIN|POLLOUT;
	fd[1].revents = 0;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sock,(struct sockaddr*)&addr,sizeof(addr)) < 0)
	{
		perror("connect");
		return -3;
	}

	int pipefd[2];
	int ret = pipe(pipefd);

	char read_buf[1024] = {0};

	while(1)
	{
		int retpo = poll(fd,2,-1);
		if(retpo < 0)
		{
			perror("poll");
			break;
		}

		if(fd[1].revents & POLLIN)
		{
			memset(read_buf,'\0',sizeof(read_buf));
			read(fd[1].fd,read_buf,sizeof(read_buf));
			printf("%s\n",read_buf);
			fd[1].revents |= ~POLLIN;

		}
		else if(fd[1].revents & POLLHUP)
		{
			printf("server close the connection!!!\n");
			break;
		}

		if(fd[0].revents & POLLIN)
		{
			int ret1 = splice(0,NULL,pipefd[1],NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
			int ret2 = splice(pipefd[0],NULL,sock,NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
		}

	}
	close(sock);
	printf("client quit!\n");
	return 0;
}
