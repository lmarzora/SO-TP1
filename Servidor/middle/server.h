#include<pthread.h>

typedef struct
{
	POKEMON pokemons[6];
	int cant;
	pthread_t id;
}TRAY;
int closeConnection(CONNECTION*);
void acceptConnection(CONNECTION*);
void createServer();
void killServer(int);
