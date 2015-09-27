//fifos.c
#include "../../commons/com.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

static int pid, n;

static int serv;
static int pid;

const static char* serverl = "/tmp/listener";
static char clsv[35];
static char svcl[35];
char semName[35];


int requestConnection(CONNECTION* c) {
	if((serv = open(serverl, O_WRONLY)) == -1)
		perror("ERROR connecting");
	
	pid = getpid();
	//printf("connecting\n");
	int n = write(serv,&pid,sizeof(int));
	close(serv);
	if(n<=0){
		perror("error");
		exit(1);	
	}
	sprintf(clsv,"/tmp/clsv-%d",pid);
	sprintf(svcl,"/tmp/svcl-%d",pid);
	

	sprintf(semName,"/sem-%d",pid);
	//printf("%s\n",semName);
	//sem_unlink(semName);
	sem_t* sem = sem_open(semName,O_CREAT,0777, 0);
	if(sem == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}
	sem_wait(sem);
	sem_close(sem);
	sem_unlink(semName);
	
	c->pid = pid;
	return 1;
}


int sendPacket(CONNECTION* c, PACKET* p,int size){
	//printf("sending packet\n");
	int fdS = open(clsv,O_WRONLY);
	if(fdS < 0 )
	{
		perror("error");
		exit(1);	
	}
	n = write(fdS, p, size);
	if(n < 0){
		perror("ERROR writing to fifo");
		exit(1);
	}
	close(fdS);
	return n;
}

int receivePacket(CONNECTION* c, PACKET* p,int size){
	//printf("receiving packet\n");
	int fdR = open(svcl,O_RDONLY);
	n = read(fdR, p,size);
	if (n < 0)
  	{
     		perror("ERROR reading from fifo");
 		exit(1);
 	}
	close(fdR);
	return n;
}


int endConnection(CONNECTION* c)
{
	remove(semName);
	remove(clsv);
	remove(svcl);
	return 1;
}



