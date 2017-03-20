#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/epoll.h>
#include<assert.h>

#define _SIZE_ 10240

typedef struct epfd
{
	int fd;
	char buf[_SIZE_];
}epfd;

epfd* getepfd(int fd)
{
	if(fd < 0)
	{
		printf("fd < 0");
		return NULL;
	}
    epfd* ret = (epfd*)malloc(sizeof(epfd));
	ret->fd = fd;
	return ret;
}

int makesock(const char* ip,const char* port)
{
	assert(ip);	assert(port);

	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		return -1;
	}

	int opt = 1;
	int retset = setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	if(retset < 0)
	{
		perror("setsockopt");
		return -2;
	}


	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ip);

	if(bind(sock,(struct sockaddr*)&addr,sizeof(addr)) < 0)
	{
		perror("bind");
		return -3;
	}

	if(listen(sock,5))
	{
		perror("listen");
		return -4;
	}

	return sock;
}

int main(int argc,char* argv[])
{	
	if(argc<3)
	{
		printf("Please input:%s [ip] [port]",argv[0]);
		return -1;
	}
	
	int listensock = makesock(argv[1],argv[2]);
	printf("%d\n",listensock);

	int fd = epoll_create(256);
	printf("Debug: %d\n",fd);
	if(fd < 0)
	{
		perror("epoll_create");
		return -2;
	}

	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.ptr = getepfd(listensock);
	int ret = epoll_ctl(fd,EPOLL_CTL_ADD,listensock,&ev);
	if(ret < 0)
	{
		perror("epoll_ctl listensock");
		free((epfd*)(ev.data.ptr));
		close(fd);
		return -3;
	}
	while(1)
	{
		int timeout = 5000;
		struct epoll_event *evs = (struct epoll_event*)malloc(sizeof(struct epoll_event)*32);
		int num = epoll_wait(fd,evs,32,timeout);

		switch(num)
		{
			case -1:
				perror("epoll_wait");
				close(fd);
				break;
			case 0:
				printf("...timeout\n");
				continue;
				break;
			default:
				{
					int i = 0;
					for(i = 0;i < num;i++)
					{

						int jfd = ((epfd*)(evs[i].data.ptr))->fd;
						if(jfd == listensock && evs[i].events & EPOLLIN)
						{
						printf("Debug listensock: %d\n",jfd);
							struct sockaddr_in peer;
							int len = sizeof(peer);
							int sock = accept(listensock,(struct sockaddr*)&peer,&len);
							printf("Get a client %s:%d(%d)\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port),sock);
							if(sock < 0)
							{
								perror("accept");
								continue;
							}
							struct epoll_event evk;
							evk.events = EPOLLIN | EPOLLET;
							evk.data.ptr = getepfd(sock);
							int retctl = epoll_ctl(fd,EPOLL_CTL_ADD,sock,&evk);
							if(retctl < 0)
							{
								perror("epoll_ctl sock");
								free((epfd*)(evk.data.ptr));
								close(sock);
								continue;
							}
						}//listensock
						else if(jfd != listensock && (evs[i].events & EPOLLIN))
						{
							//char readbuf[10240];
							int size = read(jfd,((epfd*)(evs[i].data.ptr))->buf,_SIZE_);
							if(size == 0)
							{
								printf("client is quit!!!\n");
								epoll_ctl(fd,EPOLL_CTL_DEL,jfd,NULL);
								close(jfd);
								free((epfd*)(evs[i].data.ptr));
								continue;
							}
							else if(size < 0)
							{
								perror("read");
								continue;
							}
							else
							{
								evs[i].events = EPOLLOUT | EPOLLET;
								int ret = epoll_ctl(fd,EPOLL_CTL_MOD,jfd,&evs[i]);

								(((epfd*)(evs[i].data.ptr))->buf)[size-1] = '\0';
								printf("%s\n",((epfd*)(evs[i].data.ptr))->buf);
							}
						}
						else if(jfd != listensock && (evs[i].events & EPOLLOUT))
						{
							printf("EPOLLOUT\n");
							char wrbuf[10240] = "HTTP/1.1 200 ok\r\n\r\n./server.c\n";
							int retwr = write(jfd,wrbuf,sizeof(wrbuf));
							if(retwr < 0)
							{
								perror("retwr");
							}


							//epoll_ctl(fd,EPOLL_CTL_DEL,jfd,NULL);
							//close(jfd);
							//free((epfd*)(evs[i].data.ptr));
						}
						else
						{
							///
						}
					}
					break;
				}
			

		}
		free(evs);

	}

	close(listensock);
	free((epfd*)(ev.data.ptr));
	close(fd);
	return 0;
}
