#include "../../commons/com.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>



const static char* idserver = "/my_server";
const static char * semConnectName = "/semconnect";
const static char * semStartName = "/semstart";
static char sharedmem[35];
static sem_t* semSend;
static sem_t* semReceive;
static sem_t* semconnect;
static sem_t* semstart;
static PACKET * paquete;
char semSendName[35];
char semReceiveName[35];



void fatal(char* s){
	perror(s);
	exit(1);
}

int requestConnection(CONNECTION* c) {

	semstart = sem_open(semStartName,O_CREAT,0777, 1);
	if(semstart == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}
	
	sem_wait(semstart);
	
	int fd;
	int * memid;

	if ( (fd = shm_open(idserver, O_RDWR, 0666)) == -1 )
		fatal("sh_open");
	if ( !(memid = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		fatal("mmap");
	close(fd);
	int pid = getpid();
	//printf("connecting\n");

	*memid = pid;

	semconnect = sem_open(semConnectName,O_CREAT,0777, 0);
	if(semconnect == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}
	sem_post(semconnect);

	sprintf(sharedmem,"/shmem-%d",pid);
	
	sprintf(semSendName,"/sem_clsv-%d",pid);
	sprintf(semReceiveName,"/sem_svcl-%d",pid);
	semSend = sem_open(semSendName,O_CREAT,0777, 0);
	if(semSend == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}
	semReceive = sem_open(semReceiveName,O_CREAT,0777, 0);
	if(semReceive == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}
	sem_wait(semReceive);
	
	c->pid = pid;

	//printf("me conecte\n");

	return 1;
}


int sendPacket(CONNECTION* c, PACKET* p,int size){
	//printf("sending packet from client to server\n");

	int fd;
	
	if ( (fd = shm_open(sharedmem, O_RDWR, 0666)) == -1 )
		fatal("sh_open");
	if ( !(paquete = mmap(NULL, sizeof(PACKET), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		fatal("mmap");

	memcpy(paquete, p, size);

	close(fd);

	sem_post(semSend);
	return 1;
	
}



int receivePacket(CONNECTION* c, PACKET* p,int size){
	
	sem_wait(semReceive);

	//printf("receiving packet\n");

	memcpy(p, paquete, size);

	//printf("packet received\n");
	return 1;
	
}


int endConnection(CONNECTION* c){
	sem_close(semReceive);
	sem_unlink(semReceiveName);

	sem_close(semSend);
	sem_unlink(semSendName);

	remove(semSendName);
	remove(semReceiveName);

	munmap(paquete, sizeof(PACKET));
	shm_unlink(sharedmem);
	
	return 1;
}