#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../commons/myTypes.h"
#define forever while(1)

int main(){

	POKEMON pokemones[6];
	int i;
	for(i = 0; i < 6; i++){
		pokemones[i].name = BULBASAUR;
		strcpy(pokemones[i].nick,"wakawaka");
		pokemones[i].life = i * 10;
	}

	char buffer[256];
	printf("hola\n");

	forever{
		printf("Qué desea hacer?\n");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		curarPokemones(pokemones, 6);
	
  }

}
