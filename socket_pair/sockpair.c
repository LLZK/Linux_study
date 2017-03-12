#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>

int main()
{
	int sv[2];
	int ret = socketpair(PF_LOCAL,SOCK_STREAM,0,sv);
	if(ret < 0)
	{
		perror("socketpair");
	}

	pid_t id = fork();
	if(id == 0)
	{
		close(sv[0]);
		char buf[1024];
		while(1)
		{
			sleep(1);
			memset(buf,'\0',sizeof(buf));
			strcpy(buf,"hello!");
			write(sv[1],buf,strlen(buf));
			ssize_t s = read(sv[1],buf,sizeof(buf));
			buf[s] = '\0';
			printf("Father echo# %s\n",buf);
		}
	}
	else if(id > 0)
	{
		close(sv[1]);
		char buf[1024];
		while(1)
		{
			sleep(1);
			memset(buf,'\0',sizeof(buf));
			ssize_t s = read(sv[0],buf,sizeof(buf));
			buf[s] = '\0';
			printf("Child say# %s\n",buf);
			write(sv[0],buf,strlen(buf));
		}

	}
	return 0;
}

