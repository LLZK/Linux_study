#include"comm.h"


int main()
{
	printf("test_sem\n");
	int semid = creatSem(1);
    if(semid < 0)
	{
		return -1;
	}
	int ret = initSem(semid,0);
	if(ret < 0)
	{
		return -2;
	}
	P(semid,0);
	sleep(15);
	printf("nihao \n");
	exit(1);
	printf("DEBUG:exit\n");
	V(semid,0);
	sleep(5);

	destorySem(semid);

	return 0;
}

/*
int main()
{
	printf("test_sem\n");
	int semid = creatSem(1);
    if(semid < 0)
	{
		return -1;
	}
	int ret = initSem(semid,0);
	if(ret < 0)
	{
		return -2;
	}
	pid_t id = fork();
	if(id==0)
	{
		//child
     //		printf("child proc,pid:%d,ppid:%d\n",getpid(),getppid());
	    while(1)
		{
			P(semid,0);
			usleep(13300);
			printf("A");
			fflush(stdout);
			usleep(10000);
			printf("A");
			fflush(stdout);
			V(semid,0);
		}
	}
	else
	{
		//father
	//	printf("father proc,pid:%d,ppid:%d\n",getpid(),getppid());
	    while(1)
		{
		P(semid,0);
		usleep(1123);
		printf("B");
		fflush(stdout);
		usleep(100);
		printf("B");
		fflush(stdout);
	    V(semid,0);
		}
	}
//	sleep(10);
	destorySem(semid);
	return 0;
}*/
