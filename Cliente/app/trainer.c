#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../commons/myTypes.h"
#define forever while(1)

int main(){

	srand(time(NULL));
	POKEMON pokemones[6];
	int i;
	for(i = 0; i < 6; i++){
		strcpy(pokemones[i].name, POKEMON_NAMES[rand()%150]);
		pokemones[i].life = i * 10;
	}

	char buffer[256];
	printf("hola\n");

	forever{
		printf("Qué desea hacer?\n");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		curar_pokemones(pokemones, 6);
	
  }

}
