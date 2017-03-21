#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>

int main(int argc,char* argv[])
{
	if(argc < 3)
	{
		printf("Please input :%s [ip][port]\n",argv[0]);
		return 1;
	}
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		return 2;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	int ret = connect(sock,(struct sockaddr*)&addr,sizeof(addr));
	if(ret < 0)
	{
		perror("connect");
		return 3;
	}

	char buf[1024];
	memset(buf,'\0',sizeof(buf));
	while(1)
	{
		printf("Please input#");
		fflush(stdout);
		scanf("%s",buf);
		//printf("%d\n",sizeof(buf));
		write(sock,buf,sizeof(buf));
	}
	close(sock);

	return 0;
}

