#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../commons/com/clserv.h"

static CLSVBUFF clsvbuff;

static
int
send_rcv( CLSVBUFF *p, int opcode, int qty )
{
	printf("sending rcv\n");
	printf("%d",p);
	int serv = connect_to("127.0.0.1","5001");
	int qtyrec;
	int d;
	p->opc = opcode;
	d = send_packet( p, qty + sizeof(OPC), serv);
	qtyrec = recive_packet( p, sizeof( *p ), serv);

	int ret_opcode = p->opc;
	if(ret_opcode = RET_CURAR) {
		printf("pokemones curados\n");
		
	}
	close(serv);
	return qtyrec - sizeof ( OPC );
}

int curarPokemones(POKEMON* pokemones,int cant) {

	printf("curando\n");
	CLSV_POKEMON_TRANSFER * ps;
	SVCL_POKEMON_TRANSFER * pr;
	
	ps = &clsvbuff.data.clsv_pokemon_transfer;
	pr = &clsvbuff.data.svcl_pokemon_transfer;

	memcpy(ps->pokemons,pokemones,cant*sizeof(POKEMON));
	ps->cant=cant;
	ps->id=1;
	
	send_rcv(&clsvbuff,CURAR,sizeof(CLSV_POKEMON_TRANSFER));
	int i = 0;	
	for(i;i<cant;i++) {
		pokemones[i] = pr->pokemons[i];
		printf("%s\n",pokemones[i].nick);
	}
}

/* regalar , adoptar */


