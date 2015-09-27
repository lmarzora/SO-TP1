#include<pthread.h>

typedef struct
{
	POKEMON pokemons[6];
	int cant;
	int pid;
	pthread_t id;
}TRAY;
int closeConnection(CONNECTION*);
void acceptConnection(CONNECTION*);
void createServer();
void killServer(int);
