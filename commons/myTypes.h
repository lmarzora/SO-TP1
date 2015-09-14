
#ifndef __MYDEFS__
#define __MYDEF__

#define forever while(1)
#define K 1024
#define MAX_LENGTH 256
#endif

typedef enum {
	BULBASAUR, IVYSAUR
} POKEMON_NAME;

typedef struct
{
	POKEMON_NAME name;
	char nick[MAX_LENGTH];
	int life;
} POKEMON;



typedef int CLIENT_ID;


