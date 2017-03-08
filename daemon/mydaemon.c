#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>


/*void mydaemon()
{
//	umask(0);
	pid_t id = fork();
	if(id > 0 )
	{
		exit(1);
	}

	daemon(0,0);
*
	printf("Debug\n");
	setsid();

	chdir("/");

	close(0);
	close(1);
	close(2);

	signal(SIGCHLD,SIG_IGN);

}
*/
int main()
{
	daemon(0,0);
	while(1);
	return 0;
}
