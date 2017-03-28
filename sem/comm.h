#ifndef __COMM__
#define __COMM__

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<stdlib.h>

#define PATHNAME "."
#define PROJID 0x6666 //0 ~ 258

union semun {
	     int              val;    /* Value for SETVAL */
		 struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
		 unsigned short  *array;  /* Array for GETALL, SETALL */
	  	 struct seminfo  *__buf;  /* Buffer for IPC_INFO(Linux-specific) */
         };


int initSem(int semid,int which);
int commSem(int nums,int flags);
int creatSem(int nums);
int gerSem(int nums);
int P(int semid,int which);
int V(int semid,int which);
int destorySem(int semid);
#endif // __COMM__
