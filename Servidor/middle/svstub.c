
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
		
		CLSV_POKEMON_TRANSFER * pr;
		pr = &clsvbuff.data.clsv_pokemon_transfer;
		int cant = pr->cant;
		POKEMON pokemones[cant];
		memcpy(pokemones,pr->pokemons,cant*sizeof(POKEMON));

		int ok = curar_pokemones(pokemones, cant);

		SVCL_POKEMON_TRANSFER * ps;
		ps = &clsvbuff.data.svcl_pokemon_transfer;
		memcpy(ps->pokemons,pokemones,cant*sizeof(POKEMON));
		ps->cant=cant;
		ps->ok=ok;		
		p->opc=RET_CURAR;
		
	}
	
	
	return sizeof(SVCL_POKEMON_TRANSFER)+ sizeof( OPC );
}

static
void
do_server()
{
	int qty;
	qty = receive_packet( &clsvbuff, sizeof( clsvbuff ));
	qty = process_server( &clsvbuff, qty );
	send_packet( &clsvbuff, qty );

}

void
main( void )
{
	
	creatServ();
	forever
		do_server();
}






