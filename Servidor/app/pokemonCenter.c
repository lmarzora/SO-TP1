#include "../../commons/myTypes.h"


int curar_pokemones(POKEMON *pokemones, int cant){
	int i;
	for(i = 0; i<cant; i++){
		usleep(10000* (100-pokemones[i].life));
		pokemones[i].life = 100;
	}
	return 1;
}

