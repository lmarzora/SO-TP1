#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../commons/myTypes.h"
#define forever while(1)

void generatePokemons(POKEMON pokemones[6]);
void printPokemons(POKEMON pokemones[6]);

int main(){

	POKEMON pokemones[6];
	generatePokemons(pokemones);

	printf("------------------- POKEMON CENTER -------------------\n\n");
	printf("Your pokemons are: \n");
	printPokemons(pokemones);


	char buffer[256];

	forever{
		printf("--> What do you want to do?\n$ ");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);

		if(strcmp(buffer,"heal\n") == 0){
			curar_pokemones(pokemones, 6);
		}else if (strcmp(buffer, "new pokemons\n") == 0)
		{
			generatePokemons(pokemones);
			printf("Your new pokemons are\n");
			printPokemons(pokemones);
		}else if(strcmp(buffer, "my pokemons\n") == 0){
			printPokemons(pokemones);
		}else{
			printf("Invalid command\n");
		}
		
	
  }

}

void generatePokemons(POKEMON pokemones[6]){
	srand(time(NULL));
	int i;
	int cant = rand()%3 + 4;
	for(i = 0; i < cant; i++){
		strcpy(pokemones[i].name, POKEMON_NAMES[rand()%150]);
		pokemones[i].life = rand()%70;
	}
	for(i = cant; i < 6; i++){
		pokemones[i].life = -1;
	}

}

void printPokemons(POKEMON pokemones[6]){
	int i;
	int j = 1;
	for (i= 0; i<6 && pokemones[i].life != -1; i++){
		printf("   %d) %s\t--> Life: %d%% \n", j, pokemones[i].name, pokemones[i].life);
		j++;
	}

}
