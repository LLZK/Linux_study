#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<poll.h>
#include<fcntl.h>

#define USER_SIZE 10
#define FD_SIZE 65535
#define WRITE_SIZE 1024
#define TIME_OUT -1

typedef struct client_node
{
	struct sockaddr_in addr;
	char *datatowrite;
	char buf[WRITE_SIZE];

}client_node;


int makesock(const char* ip,const char* port)
{
	assert(ip);
	assert(port);

	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		return 1;
	}

	int opt = 1;
	int retset = setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	if(retset < 0)
	{
		perror("setsockopt");
		return 2;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);

	int retbind = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
	if(retbind < 0)
	{
		perror("bind");
		return 3;
	}

	int retlis = listen(sock,5);
	if(retlis < 0)
	{
		perror("listen");
		return 4;
	}
	return sock;
}



int main(int argc,char*argv[])
{


	printf("listensock\n");
	if(argc < 3)
	{
		printf("Please input#%s [ip] [port]\n",argv[0]);

		return -1;
	}

	int listensock = makesock(argv[1],argv[2]);
	printf("listensock\n");
   struct client_node *user = (struct client_node*)malloc(sizeof(struct client_node)*FD_SIZE);


	printf("%d\n",sizeof(user));
	
	struct pollfd pfd[USER_SIZE+1];

	int i = 0;
	for(;i < USER_SIZE;i++)
	{
		pfd[i].fd = -1;
		pfd[i].events = 0;
	}
	pfd[0].fd = listensock;
	pfd[0].events = POLLIN|POLLERR;
	pfd[0].revents = 0;

	int usercounters = 1;

	while(1)
	{
		//struct timeval timeout = {5,0};
		int ret = poll(pfd,usercounters,TIME_OUT);

		if(ret < 0)
		{
			perror("poll");
			continue;
		}
		int j = 0;
		for(j = 0;j < usercounters;j++)
		{
			if((pfd[j].fd == listensock) && (pfd[j].revents & POLLIN))
			{
				struct sockaddr_in peer;
				int len = sizeof(peer);
				int sock = accept(listensock,(struct sockaddr*)&peer,&len);
				
				if(sock < 0)
				{
					perror("accept");
					close(sock);
					continue;
				}
				
				if(usercounters == USER_SIZE+1)
				{
					char buf[128] = "The network is busy,please try again later!";
					write(sock,buf,sizeof(buf));
					printf("A clinet is not connected!\n ");
					continue;
				}

				usercounters++;
				user[sock].addr = peer;
				user[sock].datatowrite = NULL;
			    //set non-blocking I/O
				//
				int old_stat = fcntl(sock,F_GETFL);
				int new_stat = old_stat | O_NONBLOCK;
				fcntl(sock,F_SETFL,new_stat);

				pfd[usercounters-1].fd = sock;
				pfd[usercounters-1].events = POLLIN|POLLERR|POLLHUP;
				pfd[usercounters-1].revents = 0;
				printf("Get a client %s:%d,(%d)\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port),sock);

			}
			else if(pfd[j].revents & POLLERR)
			{
				printf("Get an error connect\n");
				char error[32];
				int len = sizeof(error);
				memset(error,0,sizeof(error));
				if(getsockopt(pfd[j].fd,SOL_SOCKET,SO_ERROR,&error,&len)<0)
				{
					perror("getsockopt");
				}

			}
			else if(pfd[j].revents & POLLHUP)
			{
				printf("POLLHUP\n");
				printf("The %s:%d(%d) is left",inet_ntoa(user[pfd[j].fd].addr.sin_addr),ntohs(user[pfd[j].fd].addr.sin_port),pfd[j].fd);
				user[pfd[j].fd] = user[pfd[usercounters - 1].fd];
				close(pfd[j].fd);
				pfd[j] = pfd[usercounters -1];
				j--;
				usercounters--;
			}
			else if(pfd[j].revents & POLLIN)
			{
				int sockfd = pfd[j].fd;
				memset(user[sockfd].buf,'\0',WRITE_SIZE);
				int s = read(sockfd,user[sockfd].buf,sizeof(user[sockfd].buf));
				if(s < 0)
				{
					
					printf("Read %s:%d(%d) Error !!!!\n",inet_ntoa(user[pfd[j].fd].addr.sin_addr),ntohs(user[pfd[j].fd].addr.sin_port),sockfd);
					continue;
				}
				else if(s == 0)
				{
					printf("%s:%d(%d) is quit !!!!\n",inet_ntoa(user[pfd[j].fd].addr.sin_addr),ntohs(user[pfd[j].fd].addr.sin_port),sockfd);
				user[pfd[j].fd] = user[pfd[usercounters - 1].fd];
				close(pfd[j].fd);
				pfd[j] = pfd[usercounters -1];
				j--;
				usercounters--;

					continue;
				}
				else
				{//tell other sockets to write

					user[pfd[j].fd].buf[s-1] = '\0';
					int k = 1;
					int count = 1;
					for(;k < usercounters;k++)
					{
						if(sockfd != pfd[k].fd)
						{						
							
							user[pfd[k].fd].datatowrite = user[sockfd].buf;
							printf("count%d writesock %d,readsock :%d get success\n",k,sockfd,pfd[k].fd);

							pfd[k].events |= ~POLLIN;
							pfd[k].events |= POLLOUT;
						}
					}
				}
			}
			else if(pfd[j].revents & POLLOUT)
			{
			//	printf("dx");
				if(user[pfd[j].fd].datatowrite == NULL)
				{
					continue;
				}
				int ret = write(pfd[j].fd,user[pfd[j].fd].datatowrite,strlen(user[pfd[j].fd].datatowrite));
				printf("writeret: %d sock%d write success!!!\n",ret,pfd[j].fd);
				if(ret < 0)
				{
					printf(" %s:%d(%d) Write Error !!!!\n",inet_ntoa(user[pfd[j].fd].addr.sin_addr),ntohs(user[pfd[j].fd].addr.sin_port),pfd[j].fd);
					continue;
				}
				user[pfd[j].fd].datatowrite = NULL;
				pfd[j].events |= POLLIN;
				pfd[j].events |= ~POLLOUT;
				pfd[j].revents |= ~POLLOUT;
			}

		}


	}
	free(user);
	close(listensock);
	return 0;
}
