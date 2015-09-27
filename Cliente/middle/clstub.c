#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../commons/com.h"

static PACKET p;
static CONNECTION c;

int requestConnection(CONNECTION*);
int endConnection(CONNECTION*);

static
int
send_rcv(int opcode, int size)
{
	//COMENTDEBUG
	//printf("sending rcv\n");
	int s = requestConnection(&c);
	int qtyrec, qtysent;
	p.opc = opcode;
	qtysent = sendPacket(&c, &p, size + sizeof(opcode));
	qtyrec = receivePacket(&c, &p, sizeof(PACKET));
	endConnection(&c);
	int ret_opcode = p.opc;
	if(ret_opcode = RET_CURAR) {
		//COMENTDEBUG
		//printf("pokemones curados\n");
		
	}

	return qtyrec - sizeof ( OPC );
}

void killClient(int sig) {
	endConnection(&c);
	printf("\nWe hope to see you again!\n");	
	exit(1);
}


int curar_pokemones(POKEMON* pokemones,int cant) {

	CLSV_POKEMON_TRANSFER * ps;
	SVCL_POKEMON_TRANSFER * pr;
	
	ps = &p.data.clsv_pokemon_transfer;
	pr = &p.data.svcl_pokemon_transfer;

	memcpy(ps->pokemons,pokemones,cant*sizeof(POKEMON));
	ps->cant=cant;
	ps->id=getpid();

	
	send_rcv(CURAR,sizeof(CLSV_POKEMON_TRANSFER));

  	memcpy(pokemones,ps->pokemons, cant*sizeof(POKEMON));

  	return 1;


}

int regalar_pokemon(POKEMON *pokemones, int index){

	CLSV_POKEMON_TRANSFER * ps;
	SVCL_POKEMON_TRANSFER * pr;
	
	ps = &p.data.clsv_pokemon_transfer;
	pr = &p.data.svcl_pokemon_transfer;

	memcpy(&(ps->pokemons[0]),&(pokemones[index]),sizeof(POKEMON));

	ps->cant = 1;
	ps->id = getpid();

	send_rcv(REGALAR,sizeof(CLSV_POKEMON_TRANSFER));

	pokemones[index].life = -1;

	return 1;
}

int adoptar_pokemon(POKEMON * pokemon){
	CLSV_POKEMON_TRANSFER * ps;
	SVCL_POKEMON_TRANSFER * pr;
	
	ps = &p.data.clsv_pokemon_transfer;
	pr = &p.data.svcl_pokemon_transfer;

	ps->cant = 0;
	ps->id = getpid();

	send_rcv(ADOPTAR,sizeof(CLSV_POKEMON_TRANSFER));

	memcpy(pokemon, &(ps->pokemons[0]),sizeof(POKEMON));

	return 1;
}

/* regalar , adoptar */


