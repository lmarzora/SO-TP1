#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../commons/myTypes.h"
#include "../../commons/API.h"
#include <signal.h>
#include <fcntl.h>

#define forever while(1)


static int database_id_fd;
static char *database_name = "/tmp/database";

void generatePokemons(POKEMON pokemones[6]);
void printPokemons(POKEMON pokemones[6]);
void printLogo();
void printHelp();
void killClient(int);
void cmd_curar_pokemones(POKEMON pokemones[6]);
void cmd_new_pokemones(POKEMON pokemones[6]);
void cmd_abandon(POKEMON pokemones[6], char * buffer);
void cmd_adopt(POKEMON pokemones[6]);


void requestConnection();
void endConnection();



int cant_pokemones;
int saldo;

int main(){


	srand(time(NULL));

//	signal(SIGINT,killClient);
	POKEMON pokemones[MAX_NUM_POKEMONS];
	generatePokemons(pokemones);

	printf("\033[2J\033[1;1H");
	printLogo();
	printf("\t\t    ---- POKEMON CENTER ----\n");
	printf("\t\t   Llorca, Marzoratti, Vitali\n");
	printf("Your pokemons are: \n");
	printPokemons(pokemones);
	int continue_trainer = 1;

	char buffer[256];

	while(continue_trainer){
		printf("\n--> What do you want to do?\n$ ");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);

		if(strcmp(buffer,"heal\n") == 0)
		{
			cmd_curar_pokemones(pokemones);
		}else if (strcmp(buffer, "new pokemons\n") == 0){
			cmd_new_pokemones(pokemones);
		}else if(strcmp(buffer, "my pokemons\n") == 0){
			printPokemons(pokemones);
		}else if((strcmp(buffer, "help\n")) == 0 || (strcmp(buffer, "man\n")) == 0){
			printHelp();
		}else if(strcmp(buffer, "abandon\n") == 0){
			cmd_abandon(pokemones, buffer);
		}else if(strcmp(buffer, "adopt\n")== 0){
			cmd_adopt(pokemones);
		}else if(strcmp(buffer, "exit\n")== 0){
			continue_trainer = 0;
//			killClient(0);
		}else{
			printf("Invalid command\n");
		}	
  }

}

void cmd_curar_pokemones(POKEMON pokemones[6]){
	int i, cant = 0;
	for(i=0; i< MAX_NUM_POKEMONS; i++){
		int life = pokemones[i].life;
		if(life == 100){
			cant++;
		}
	}
	if(cant == cant_pokemones){
		printf("Your Pokemons are already fully healed!\n");
		return;
	}
	curar_pokemones(pokemones, MAX_NUM_POKEMONS);
	printf("Thank you for waiting\n");
	printf("We've restored your Pokemons to full health\n");
}

void cmd_new_pokemones(POKEMON pokemones[6]){
	generatePokemons(pokemones);
	printf("Your new Pokemons are\n");
	printPokemons(pokemones);
}

void cmd_abandon(POKEMON pokemones[6], char * buffer){
	requestConnection();
	if(cant_pokemones != 1){
		printf("Which one do you want to leave?\n\n");
		printPokemons(pokemones);
		fgets(buffer, 255, stdin);

		int n = buffer[0] - '0';
		if(buffer[1] != '\n' || n < 1 || n > cant_pokemones){
			printf("Invalid number\n");
			return;
		}
		n--;
		//printf("n = %d\n", n);
		int i;
		int j = 0;
		for(i = 0; i < MAX_NUM_POKEMONS; i++){
			if(pokemones[i].life != -1){
				if(j == n){
					printf("Goodbye %s!\n", pokemones[i].name);
					regalar_pokemon(pokemones, i);
					pokemones[i].life = -1;
					cant_pokemones--;
					saldo++;
				}
				j++;
			}
		}	
	}else
		printf("You only have one Pokemon left\n");
	endConnection();
}

void cmd_adopt(POKEMON pokemones[6]){
	requestConnection();
	if(cant_pokemones == MAX_NUM_POKEMONS){
		printf("You don't have room for any more Pokemons\n");
	}else if(saldo <=0){
		printf("You need to abandon before adopting\n");
	}else{
		int i;
		for(i=0; i<MAX_NUM_POKEMONS && pokemones[i].life != -1; i++);
		adoptar_pokemon(&pokemones[i]);
		printf("Congratulations! You just adopted: %s\n", pokemones[i].name);
		saldo--;
		cant_pokemones++;
	}
	endConnection();
}

void generatePokemons(POKEMON pokemones[6]){
	int i;
	int cant = rand()%3 + 4;
	for(i = 0; i < cant; i++){
		strcpy(pokemones[i].name, POKEMON_NAMES[rand()%151]);
		pokemones[i].life = rand()%70 + 1;
	}
	for(i = cant; i < 6; i++){
		pokemones[i].life = -1;
	}
	cant_pokemones = cant;
	saldo = 0;
}

void printPokemons(POKEMON pokemones[6]){
	int i;
	int j = 1;
	for (i= 0; i<MAX_NUM_POKEMONS; i++){
		if(pokemones[i].life != -1){
			printf("   %d) %-15s---   Life: %d%% \n", j, pokemones[i].name, pokemones[i].life);
			j++;
		}
	}

}

void requestConnection(){
	if((database_id_fd = open(database_name, O_RDWR|O_CREAT, 0666)) == -1){
		perror("fail opening database");
	}

	struct flock lock;

 memset (&lock, 0, sizeof(lock));
	lock.l_type = F_WRLCK;
	fcntl(database_id_fd, F_SETLKW, &lock);
	printf("estoy en lock\n");

}

void endConnection(){

	struct flock lock;
 memset (&lock, 0, sizeof(lock));
	lock.l_type = F_UNLCK;
	fcntl (database_id_fd, F_SETLKW, &lock);
	printf("sali del lock\n");

	close (database_id_fd);
}







void printLogo(){
	printf("                                .::.                          \n");
	printf("                              .;:**'                          \n");
	printf("                              `                               \n");
	printf("  .:XHHHHk.              db.   .;;.     dH  MX                \n");
	printf("oMMMMMMMMMMM       ~MM  dMMP :MMMMMR   MMM  MR      ~MRMN     \n");
	printf("QMMMMMb  \"MMX       MMMMMMP !MX' :M~   MMM MMM  .oo. XMMM 'MMM\n");
	printf("  `MMMM.  )M> :X!Hk. MMMM   XMM.o\"  .  MMMMMMM X?XMMM MMM>!MMP\n");
	printf("   'MMMb.dM! XM M'?M MMMMMX.`MMMMMMMM~ MM MMM XM `\" MX MMXXMM \n");
	printf("    ~MMMMM~ XMM. .XM XM`\"MMMb.~*?**~ .MMX M t MMbooMM XMMMMMP \n");
	printf("     ?MMM>  YMMMMMM! MM   `?MMRb.    `\"\"\"   !L\"MMMMM XM IMMM  \n");
	printf("      MMMX   \"MMMM\"  MM       ~%%:           !Mh.\"\"\" dMI IMMP  \n");
	printf("      'MMM.                                             IMX   \n");
	printf("       ~M!M                                             IMP   \n");
}

void printHelp(){
	printf("\n***\tHelp\t***\n");
	printf("Available commands:\n");
	printf("--> heal: Heal all your pokemons to full health\n");
	printf("--> my pokemons: Check your pokemon's health\n");
	printf("--> abandon: Allows you to put a pokemon up for adoption\n");
	printf("    Asks for a number of the one you choose to give\n");
	printf("--> adopt: Adopt a pokemon from the PokemonCenter\n");
	printf("--> new pokemons: Receive new pokemons\n");
	printf("--> exit: Exit Pokemon Center\n");
}
