#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Please input:%s [ip][port]\n",argv[1]);
		return -1;
	}

	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock < 0)
	{
		perror("socket");
		return -2;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	char buf[1024];
	memset(buf,0,sizeof(buf));
	while(1)
	{
		printf("Please Enter#");
		fflush(stdout);
		int len = read(0,buf,sizeof(buf));
		ssize_t size = sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&addr,sizeof(addr));
		memset(buf,0,sizeof(buf));
	}
	close(sock);
	return 0;
}
