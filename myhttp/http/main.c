#include"httpd.h"

void* thread_mechine(void* arg)
{
   int sock=(int)arg;
   if(pthread_detach(pthread_self()) != 0)
   {
	   print_log("pthread_detach error---",FATAL,__LINE__);
	   close(sock);
	   return NULL;
   }
   
   Dealwithsock(sock);

   return NULL;
}


int main(int argc,char* argv[])
{
	if(argc < 3)
	{
		printf("Please input: %s [ip] [port]\n",argv[0]);
		exit(1);
	}

	int listensock = startup(argv[1],argv[2]);
	//daemon(1,0);

	if(listensock < 0)
	{
		print_log("startuop error---",WARNING,__LINE__);
		exit(2);
	}
   struct sockaddr_in addr;
   socklen_t len = sizeof(addr);
	while(1)
	{
		sleep(1);
		int sock = accept(listensock,(struct sockaddr*)&addr,&len);
		if(sock < 0)
		{
			print_log("accept error---",WARNING,__LINE__);
			continue;
		}
		pthread_t tid;
		if(pthread_create(&tid,NULL,thread_mechine,(void*)sock) < 0)
		{
			print_log("pthread_create error---",FATAL,__LINE__);
			close(sock);
			continue;
		}
	}
	return 0;
}
