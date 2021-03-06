#include "../../commons/com.h"
#include "../../commons/API.h"
#include "server.h"
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <time.h>

static char* nurseS;
static char* nurseR;
static int nurse_id = 1;
static pthread_t ntid[3];
static sem_t nursePillow, healingMachine, pipeS , pipeR;
static pthread_mutex_t connectionMutex;
static sem_t * sem_guarderia;
static char * sem_guarderia_name = "/mutexcenter";

#define DONE 1

int processPacket(PACKET*);
void createNurses();
void doServer();
void* handleConnection(void*);
void doNurse(int);
void* nurse();
void fatal(char*);
void done(int);
void endServer(int);
void printList();
int print_regalar_pokemon(POKEMON * pokemon, int index, int id);
int print_adoptar_pokemon(POKEMON * pokemon, int id);

int main (void )
{
	srand(time(NULL));
	signal(SIGINT,endServer);
	createNurses();
	createServer();
	printf("-----------------------------------------------------------\n");
	printf("Num (heal) = Number of machine healing pokemon\n");
	printf("Num (abandon/adopt) = Number of pokemon up for adoption\n");
	printf("-----------------------------------------------------------\n");
	printf("%-7s %-10s %-5s %-15s %s\n", "PID/fd", "Command", "Num", " Name", "Life");
	printf("-----------------------------------------------------------\n");
	pthread_mutex_init(&connectionMutex,NULL);

	if( !(sem_guarderia = sem_open(sem_guarderia_name, O_RDWR | O_CREAT, 0666, 1)))
		fatal("Error creando semaforo guarderia\n");

	while(1)
	{
		doServer();
	}

}

void createNurses() 
{		
	//printf("creating nurses\n");
	nurseS = "/tmp/nurseS";
	if ( access(nurseS, 0) == -1 && mknod(nurseS, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod  nurseS");
	nurseR = "/tmp/nurseR";
	if ( access(nurseR, 0) == -1 && mknod(nurseR, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod nurseR");

		int i;
		sem_init(&nursePillow,0,0);
		sem_init(&healingMachine,0,3);
		sem_init(&pipeS,0,1);
		sem_init(&pipeR,0,1);
			
		for( i = 0 ; i < 3 ; i++ ){
			pthread_create(	&(ntid[i]) , NULL , nurse , NULL );
			printf("Created Nurse #%d\n", i+1);
		}
		
	return ;
}

void* nurse()
{
	pthread_mutex_lock(&connectionMutex);
	int id = nurse_id++;
	pthread_mutex_unlock(&connectionMutex);
	while(1)
		doNurse(id);
}


void doNurse(int id)
{

	TRAY tray;

	//printf("nurse sleeping\n");
	sem_wait(&nursePillow);
	//printf("wake up!\n");
	int rfd, wfd;
	
	//read from nurseR
	//printf("reading from nurseR thread: %d\n",pthread_self());
	rfd = open(nurseR,O_RDONLY);
	int n;
	n = read(rfd,&tray,sizeof(TRAY));
	if(n<0) {
		perror("ERROR READING");
		exit(1);
	}
	close(rfd);
	//printf("done reading thread: %d\n",pthread_self());
	sem_post(&pipeS);
	//heal
	int aux = tray.pid*10 + id;
	curar_pokemones(tray.pokemons, aux);
	
	//write in nurseS
	//printf("writing on nurseS thread: %d\n",pthread_self());
	sem_wait(&pipeR);
	pthread_kill(tray.id,DONE);
	wfd = open(nurseS,O_WRONLY);
	n = write(wfd,&tray,sizeof(TRAY));
	if(n<0) {
			perror("ERROR WRITING");
			exit(1);
	}
	//printf("done writing thread: %d\n",pthread_self());
	close(wfd);
	
}	


void doServer() 
{
	//printf("hi i'm a server\n");
	CONNECTION c; 
	pthread_mutex_lock(&connectionMutex);
	acceptConnection(&c);
	printf("%-7d START\n", c.pid);
	pthread_t stid;
	pthread_create(&(stid),NULL,&handleConnection,(void*)&c);
	//printf("there's a new thread\n");
	pthread_detach(stid);
}

void* handleConnection(void* s)
{
	signal(DONE,done);
	//printf("hello\n");
	CONNECTION c;
	memcpy(&c,s,sizeof(CONNECTION));
	
	pthread_mutex_unlock(&connectionMutex);
	PACKET p;
	//transport
	receivePacket( &c , &p, sizeof(PACKET));
	int size = processPacket( &p );
	//transport
	sendPacket( &c , &p, size);
	endConnection( &c );
	printf("%-7d END\n", c.pid);
	int ret = 1;
	//printf("die thread die\n");
	pthread_exit(&ret);
}

void done(int sig)
{
	//printf("DONE\n");
	signal(DONE,done);
}


int processPacket( PACKET* p )
{
	int opc = p->opc;
	//printf("opc: %d\n",opc);
	TRAY tray;
	CLSV_POKEMON_TRANSFER* pr;
	SVCL_POKEMON_TRANSFER* ps;
	
	pr =(CLSV_POKEMON_TRANSFER*) &( p->data.clsv_pokemon_transfer);
	ps =(SVCL_POKEMON_TRANSFER*) &( p->data.svcl_pokemon_transfer);		
	
	switch( opc ) 
	{
		case CURAR:
		{	

			//printf("packet : \n");
			//printf("pr->cant:%d\n",pr->cant);
			tray.cant = pr->cant;
			memcpy(tray.pokemons,pr->pokemons,tray.cant*sizeof(POKEMON));

			tray.id = pthread_self();
			tray.pid = pr->id;
			
			sem_wait(&healingMachine);
			int wfd, rfd;			
			
			
			//write nurseR;

			sem_wait(&pipeS);
			//printf("writing on nurseR thread: %d\n",pthread_self());
			sem_post(&nursePillow);
			wfd = open(nurseR,O_WRONLY);
			int n = write(wfd,&tray,sizeof(TRAY));
			//printf("n: %d\n",n);
			if(n<0) {
				perror("ERROR WRITING");
				exit(1);
			}
			close(wfd);
			//printf("done writing thread: %d\n",pthread_self());
			//printf("ZZZZZZZZ thread: %d\n",pthread_self());
			

			pause();
			//printf("up thread: %d\n",pthread_self());
			//read nurseS;

			rfd = open(nurseS,O_RDONLY);
			//printf("reding from nurseS thread: %d\n",pthread_self());
			n = read(rfd,&tray,sizeof(TRAY));
			if(n<0) {
				perror("ERROR READING");
				exit(1);
			}
			close(rfd);
			
			//printf("done reading thread: %d\n",pthread_self());
			sem_post(&pipeR);
			sem_post(&healingMachine);
				

			memcpy(pr->pokemons,tray.pokemons,tray.cant*sizeof(POKEMON));
			p->opc = RET_CURAR;
			
			break;
		}
		case ADOPTAR:
		{
			//printf("recibi pedido de adopción\n");
			POKEMON aux;
			sem_wait(sem_guarderia);
			print_adoptar_pokemon(&aux, pr->id);
			sem_post(sem_guarderia);
			memcpy(&(ps->pokemons[0]), &aux, sizeof(POKEMON));
			//printList();
			break;			
		}
		case REGALAR:
		{
			sem_wait(sem_guarderia);
			print_regalar_pokemon(pr->pokemons, 0, pr->id);
			sem_post(sem_guarderia);
			//printList();
			break;
		}
		default:
		{
			//error;
		}
	}
	return sizeof(SVCL_POKEMON_TRANSFER) + sizeof(opc);
}

void endServer(int sig)
{
	remove(nurseS);	
	remove(nurseR);
	sem_close(sem_guarderia);
	sem_unlink(sem_guarderia_name);
	remove(sem_guarderia_name);
	killServer(sig);
	exit(1);
}
/*
void
fatal(char *s)
{
	perror(s);
	exit(1);
}*/
