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


const static char *idserver = "/my_server";
const static char * semConnectName = "/semconnect";
static int * memid_listener;

void fatal(char*);

void createServer(){

	int fd;

	if ( (fd = shm_open(idserver, O_RDWR|O_CREAT, 0666)) == -1 )
		fatal("sh_open");
	ftruncate(fd, sizeof(int));
	if ( !(memid_listener = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		fatal("mmap");

	close(fd);

	printf("Server born\n");

	return;
}

int sendPacket(CONNECTION *c, PACKET *p,int size){
	
	//printf("sending packet from server to client\n");
	int n;

	char sharedmem[35];

	sprintf(sharedmem,"/shmem-%d",c->pid);

	int fd;
	PACKET * paquete;

	if ( (fd = shm_open(sharedmem, O_RDWR, 0666)) == -1 )
		fatal("sh_open");
	if ( !(paquete = mmap(NULL, sizeof(PACKET), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		fatal("mmap");

	memcpy(paquete, p, size);

	close(fd);

	char semName[35];
	sprintf(semName,"/sem_svcl-%d",c->pid);
	sem_t* sem = sem_open(semName,O_CREAT,0777,0);
	if(sem == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}
	sem_post(sem);

	return 1;

}


int receivePacket(CONNECTION* c, PACKET *p, int size){
	char semName[35];
	sprintf(semName,"/sem_clsv-%d",c->pid);
	sem_t* sem = sem_open(semName,O_CREAT,0777,0);
	if(sem == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}
	sem_wait(sem);

	//printf("receiving packet\n");
	int n;

	char sharedmem[35];

	sprintf(sharedmem,"/shmem-%d",c->pid);

	int fd;
	PACKET * paquete;

	if ( (fd = shm_open(sharedmem, O_RDWR, 0666)) == -1 )
		fatal("sh_open");
	if ( !(paquete = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		fatal("mmap");

	memcpy(p, paquete, size);

	close(fd);
	//printf("packet received\n");
	return 1;
	
}

int endConnection(CONNECTION *c){
	return 1;
}

void acceptConnection(CONNECTION *c){

	sem_t* semconnect = sem_open(semConnectName,O_CREAT,0777,0);
	if(semconnect == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}
	sem_wait(semconnect);
	sem_close(semconnect);
	//sem_unlink(semConnectName);

	c->pid = *memid_listener;
	
	//printf("pid = %d\n", c->pid);

	char sharedmem[35];

	//printf("connecting to %d\n",c->pid);

	sprintf(sharedmem,"/shmem-%d",c->pid);
	//printf("%s\n",sharedmem);

	char semName[35];
	sprintf(semName,"/sem_svcl-%d",c->pid);
	//printf("%s\n",semName);
	sem_t* sem = sem_open(semName,O_CREAT,0777,0);
	if(sem == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}

	int fd;
	PACKET * mem;
	if ( (fd = shm_open(sharedmem, O_RDWR|O_CREAT, 0666)) == -1 )
		fatal("sh_open");
	ftruncate(fd, sizeof(PACKET));
	if ( !(mem = mmap(NULL, sizeof(PACKET), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		fatal("mmap");
	close(fd);

	sem_post(sem);	

}


void killServer(int signo){
	printf("\nKilling server --- GoodBye!\n");
	munmap(memid_listener, sizeof(int));
	//sem_close(semconnect);
	sem_unlink(semConnectName);
	remove(semConnectName);
	exit(1);

}

void
fatal(char *s)
{
	remove(semConnectName);
	perror(s);
	exit(1);
}