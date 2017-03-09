#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void sigrun(int signo)
{
	//do nothing
}

int mysleep(int secs)
{
	sigset_t sig,osig;
	sigemptyset(&sig);
	sigaddset(&sig,14);
	sigprocmask(SIG_BLOCK,&sig,&osig);

	signal(SIGALRM,sigrun);

	alarm(secs);
	//pause();
	sigsuspend(&osig);
	int ret = alarm(0);
	signal(SIGALRM,SIG_DFL);
	sigprocmask(SIG_SETMASK,&osig,NULL);
	return ret;

}


int main()
{
	while(1)
	{
		mysleep(1);
		printf("hello\n");
	}
}
