#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
int main(int argc,char* argv[])
{

	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	int ret = connect(sock,(struct sockaddr*)&addr,sizeof(addr));
	if(ret < 0)
	{
		perror("connect");
		return -2;
		//break;
	}
	char buf[1024];
	while(1)
	{
		printf("Please Enter#");
		fflush(stdout);
		memset(buf,'\0',sizeof(buf));
		read(0,buf,sizeof(buf));
		
		write(sock,buf,strlen(buf));
	//	int s = read(sock,buf,sizeof(buf));
	//	buf[s-1] = '\0';
	//	printf("server echo# %s\n",buf);
	}
	close(sock);
	//printf("hello client\n");
	return 0;
}
