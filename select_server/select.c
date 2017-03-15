#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<unistd.h>
#include<assert.h>
#define _SIZE_ 128
int makesock(const char* ip,const char* port)
{
	assert(ip);
	assert(port);
	int listensock = socket(AF_INET,SOCK_STREAM,0);
	if(listensock < 0)
	{
		perror("socket");
		return 2;
	}

	int opt = 1;
	int retset = setsockopt(listensock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	if(retset < 0)
	{
		perror("setsockopt");
		return 3;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);

	if(bind(listensock,(struct sockaddr*)&addr,sizeof(addr)) < 0)
	{
		perror("bind");
		return 4;
	}

	if(listen(listensock,5) < 0)
	{
		perror("listen");
		return 5;
	}

	
	return listensock;
}


int main(int argc,char* argv[])
{


	//printf("%d",sizeof(fd_set));

	if(argc < 3)
	{
		printf("Please input:%s [ip] [port]\n",argv[0]);
		return 1;
	}
	int listensock = makesock(argv[1],argv[2]);

	int gfds[_SIZE_];
	int i = 0;
	for(i = 0;i < _SIZE_;i++)
	{
		gfds[i] = -1;
	}
	gfds[0] = listensock;

	int max_fd = gfds[0];

	while(1)
	{
		fd_set rfds;
		FD_ZERO(&rfds);
		int j = 0;
		for(;j < _SIZE_;j++)
		{
			if(gfds[j] != -1)
			{
				FD_SET(gfds[j],&rfds);
			}
			if(max_fd < gfds[j])
			{
				max_fd = gfds[j];
			}
		}

		struct timeval timeout = {5,0};

		switch(select(max_fd+1,&rfds,NULL,NULL,&timeout))
		{
			
		case -1://error
			perror("select");
			break;
		case 0://timeout
			printf("...timeout\n");
			break;
		default:
			{
			int k = 0;
			for(;k < _SIZE_;k++)
			{
				if(gfds[k] != -1 && FD_ISSET(gfds[k],&rfds))
				{
					if(gfds[k] == listensock)
					{
						
						struct sockaddr_in peer;
						int len = 0;
						memset(&peer,'\0',sizeof(peer));
						int sock = accept(listensock,(struct sockaddr*)&peer,&len);

						if(sock < 0)
						{
							perror("accept");
							continue;
						}

						printf("%s:%d(%d)",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port),sock);
						int m = 0;
						for(;m < _SIZE_;m++)
						{
							if(gfds[m] == -1)
							{
								gfds[m] = sock;
								break;
							}
						}

						if(m == _SIZE_)
						{
							printf("gfds have no space!\n");
							continue;
						}
					}
					else
					{
						char buf[1024];
						ssize_t s = read(gfds[k],buf,sizeof(buf));
						if(s < 0)
						{
							perror("read");
							continue;
						}
						else if(s == 0)
						{
							printf("client is quit!\n");
							close(gfds[k]);
							gfds[k] = -1;
							continue;
						}
						else
						{
							buf[s] = '\0';
							printf("client say# %s\n",buf);
						}

					}

				}

			}
			break;
			}
			
		}
		
	}
	close(listensock);
	
	return 0;

}
