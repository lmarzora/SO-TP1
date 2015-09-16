#include "../../commons/com/clsv.h"
#include "../../commons/API.h"
#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>
#include <sys/stat.h>

static char* nurseS;
static char* nurseR;

static pthread_t ntid[3];
static sem_t nursePillow, pillow , healingMachine, pipeS , pipeR;


int processPacket(PACKET*);
void createNurses();
void doServer();
void handleConnection(CONNECTION);
void doNurse();
void* nurse();
void fatal(char*);

int main (void )
{
	createNurses();
	createServer();

	while(1)
	{
		doServer();
	}

}

void createNurses() 
{		
	char* nurseS = "/tmp/nurseS";
	if ( access(nurseS, 0) == -1 && mknod(nurseS, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod  nurseS");
	char* nurseR = "/tmp/nurseR";
	if ( access(nurseR, 0) == -1 && mknod(nurseR, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod nurseR");
	
	int pid = fork();
		if(pid==0) 
		{
			int i;
			sem_init(&nursePillow,1,0);
			sem_init(&pillow,1,0);
			sem_init(&healingMachine,1,3);
			sem_init(&pipeS,1,1);
			sem_init(&pipeR,1,1);

			for( i = 0 ; i < 3 ; i++ )
				pthread_create(	&(ntid[i]) , NULL , &nurse , NULL );
		}
		
	return ;
}

void* nurse()
{
	while(1)
		doNurse();
}


void doNurse()
{

	TRAY tray;
	sem_wait(&nursePillow);

	int rfd, wfd;
	
	//read from nurseR
	
	rfd = open(nurseR,O_RDONLY);
	int n;
	n = read(rfd,&tray,sizeof(TRAY));
	close(rfd);
	sem_post(&pipeS);
	//heal
	heal(&tray,sizeof(TRAY));
	
	//write in nurseS

	sem_wait(&pipeR);
	wfd = open(nurseS,O_WRONLY);
	n = write(wfd,&tray,sizeof(TRAY));
	close(wfd);

	
	sem_post(&pillow);
	
}	


void doServer() 
{
	CONNECTION c; 
	c = acceptConnection();
		
	int pid = fork();
	if( pid == 0)
		handleConnection( c );
	closeConnection( c );
	
}

void handleConnection( CONNECTION c )
{
	PACKET p;
	//transport
	receivePacket( c , &p , sizeof(PACKET) );
	processPacket( &p );
	//transport
	sendPacket( c , &p , sizeof(PACKET) );
	closeConnection( c );
	exit(1);
}

int processPacket( PACKET* p )
{
	int opc = p->opc;
	TRAY tray;
	switch( opc ) 
	{
		case CURAR:
		{	
			sem_wait(&healingMachine);
			int wfd, rfd;			
			
			//write nurseR;

			sem_wait(&pipeS);
			wfd = open(nurseR,O_WRONLY);
			write(wfd,&tray,sizeof(TRAY));
			close(wfd);

			sem_post(&nursePillow);
			sem_wait(&pillow);
			
			//read nurseS;

			rfd = open(nurseS,O_RDONLY);
			read(rfd,&tray,sizeof(TRAY));
			close(rfd);
			sem_post(&pipeR);
			sem_post(&healingMachine);
			
			break;
		}
		case ADOPTAR:
		{
			//call adoptar;
			break;			
		}
		case REGALAR:
		{
			//call regalar;
			break;
		}
		default:
		{
			//error;
		}
	}
}

void
fatal(char *s)
{
	perror(s);
	exit(1);
}
