#include "../../commons/myTypes.h"
#include <stdio.h>
#include <unistd.h>

void stdprint(int pid, char * cmd, int num, char* name, int life){
	printf("%-7d %-10s %-5d %-15s %d%%\n", pid, cmd, num, name, life);
}

int curar_pokemones(POKEMON *pokemones, int id){
	int i;
	int pid = id/10;
	int machine = id%10;
	for(i = 0; i<MAX_NUM_POKEMONS; i++){
		if(pokemones[i].life != -1){
			if(id<=MAX_NUM_POKEMONS){
				printf("Curando a: %s\n", pokemones[i].name );
			}else{
				stdprint(pid, "heal", machine, pokemones[i].name, pokemones[i].life);
			}
			usleep(15000* (100-pokemones[i].life));
			pokemones[i].life = 100;
		}
	}
	return 1;
}

