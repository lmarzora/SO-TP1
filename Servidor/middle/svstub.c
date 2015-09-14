
#include<string.h>
#include "../../commons/com/clserv.h"
#include "../../commons/API.h"
#include<stdio.h>
static CLSVBUFF clsvbuff;

static
int
process_server ( CLSVBUFF*p,int qty )
{

	printf("server processing\n");
	int opcode;
	opcode = p->opc;
	if(opcode == CURAR) {
		usleep(900000);
		CLSV_POKEMON_TRANSFER * pr;
		pr = &clsvbuff.data.clsv_pokemon_transfer;
		int cant = pr->cant;
		POKEMON pokemones[cant];
		memcpy(pokemones,pr->pokemons,cant*sizeof(POKEMON));
		int i = 0;
		for(i;i<cant;i++) {
			printf("%s\n",pokemones[i].nick);
			strcpy(pokemones[i].nick,"pepe");
		}
		SVCL_POKEMON_TRANSFER * ps;
		ps = &clsvbuff.data.svcl_pokemon_transfer;
		memcpy(ps->pokemons,pokemones,cant*sizeof(POKEMON));
		i = 0;
		for(i;i<cant;i++) {
			printf("%s\n",ps->pokemons[i].nick);
		}
		ps->cant=cant;
		ps->ok=1;		
		p->opc=RET_CURAR;
		
	}
	
	
	return sizeof(SVCL_POKEMON_TRANSFER)+ sizeof( OPC );
}

static
void
do_server( int serv )
{
	int qty,cli;
	cli = receive_packet( &clsvbuff, sizeof( clsvbuff ), serv);
	qty = process_server( &clsvbuff, qty );
	send_packet( &clsvbuff, qty , cli);

}

void
main( void )
{
	int serv = creatServ();
	forever
		do_server(serv);
}



