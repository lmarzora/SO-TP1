//fifos.c
#include "../../commons/com.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include<unistd.h>

const static char* listener = "/tmp/listener";
const static char* clsv = "/tmp/clsv";
const static char* svcl = "/tmp/svcl";

static int fdSvCl;
static int fdClSv;




void fatal(char*);
void keep_waiting(int s);


void createServer(){

	printf("creating serv\n");
	
	if ( access(listener, 0) == -1 && mknod(listener, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod listener");
	//fdClSv = open(pipeClSv, O_RDONLY);

	printf("server born\n");


	return;
}


int sendPacket(CONNECTION *c, PACKET *p,int size){
	printf("sending packet\n");
	int n;
	char fifoS[35];
	sprintf(fifoS,"/tmp/svcl-%d",c->pid);
	printf("%s\n",fifoS);
	int fdS = open(fifoS,O_WRONLY);
	n = write(fdS,p,size);
	close(fdS);
	return n;
}

int receivePacket(CONNECTION* c, PACKET *p, int size){
	int qty;

	char fifoR[35];
	sprintf(fifoR,"/tmp/clsv-%d",c->pid);
	printf("%s\n",fifoR);
	int fdR = open(fifoR,O_RDONLY);
	qty = read(fdR,p,sizeof(PACKET));
	close(fdR);


	printf("packet received\n");
	return qty;
}


int endConnection(CONNECTION *c){


	return 1;
}

void acceptConnection(CONNECTION *c){
	int lisfd = open(listener,O_RDONLY);
	int n = read(lisfd,&(c->pid),sizeof(int));
	if(n<=0){
		perror("error");
	}
		
	printf("connecting to %d\n",c->pid);
	

	char fifoS[35], fifoR[35];
	sprintf(fifoR,"/tmp/clsv-%d",c->pid);
	printf("%s\n",fifoS);
	sprintf(fifoS,"/tmp/svcl-%d",c->pid);
	printf("%s\n",fifoR);
	
	char semName[35];
	sprintf(semName,"/sem-%d",c->pid);
	printf("%s\n",semName);
	//sem_unlink(semName);
	sem_t* sem = sem_open(semName,O_CREAT,0777,0);
	if(sem == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}
	sem_post(sem);
	sem_close(sem);

	if ( access(fifoS, 0) == -1 && mknod(fifoS, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod fifoS");
	if ( access(fifoR, 0) == -1 && mknod(fifoR, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod fifoR");

	close(lisfd);

	return;
}

void keep_waiting(int s){
	printf("waiting for trainers\n");
}


void killServer(int signo)
{
	printf("killing server\n");
	remove(listener);
	exit(1);
}

