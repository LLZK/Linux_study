#include<stdio.h>
#include<stdlib.h>
#include<signal.h>


void sigrun(int signo)
{
	printf("Catch the signo %d\n",signo);
}

int main()
{
	pid_t id = fork();
	if(id == 0)
	{
		//child
		//
		exit(1);
		while(1)
	{
			sleep(1);
			printf("child is Running\n");
		}
	}
	else
	{
		signal(SIGCHLD,sigrun);
		while(1)
		{
			sleep(1);
			printf("father is RUnning\n");
		}
		waitpid(id,0,NULL);
	}

}
