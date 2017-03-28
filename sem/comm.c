#include"comm.h"

int commSem(int nums,int flags)
{
	key_t _k=ftok(PATHNAME,PROJID);
	if(_k < 0)
	{
	perror("ftok");
		return -1;
	}
	int semid = semget(_k,nums,flags);
	if(semid < 0)
	{
		perror("semget");
		return -2;
	}
	return semid;
}
int creatSem(int nums)
{
	return commSem(nums,IPC_CREAT | IPC_EXCL | 0666);
}
int gerSem(int nums)
{
	return commSem(nums,IPC_CREAT);
}

int initSem(int semid,int which)
{
    union semun sem;
	sem.val = 1;
	if(semctl(semid,which,SETVAL,sem) < 0)
	{
		perror("initSem_semctl");
		return -1;
	}
	return 0;
}


int op_sem(int semid,int op,int which)
{
	struct sembuf s;
	s.sem_num = which;
	s.sem_op = op;
	s.sem_flg = SEM_UNDO;
	
	return semop(semid,&s,1);
}

int P(int semid,int which)
{
	if(op_sem(semid,-1,which) < 0)
	{
		perror("P_op_sem");
		return -1;
	}
	return 0;
}
int V(int semid, int which)
{
	if(op_sem(semid,1,which) < 0)
	{
		perror("V_op_sem");
		return -1;
	}
	return 0;
}
int destorySem(int semid)
{
	if(semctl(semid,0,IPC_RMID) < 0)
	{
		perror("semctl Destory");
		return -1;
	}
	return 0;
}

