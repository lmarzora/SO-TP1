#include "../../commons/myTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>

typedef struct node
{
	POKEMON pokemon;
	struct node * next;
} node_t;

node_t * head;


static int database_fd;
static char *database_name = "/tmp/database";

int cant;

int print_regalar_pokemon(POKEMON * pokemon, int index, int id){
	int aux = regalar_pokemon(pokemon, index);
	stdprint(id, "abandon", cant, pokemon[index].name, pokemon[index].life);
	return aux;

}
int regalar_pokemon(POKEMON * pokemon, int index){

	if((database_fd = open(database_name, O_WRONLY|O_APPEND, 0666)) == -1){
		perror("fail opening database");
	}

	write(database_fd, pokemon[index].name, strlen(pokemon[index].name));
	write(database_fd, ";", strlen("\n"));

	char buff[30];
	sprintf(buff,"%d",pokemon[index].life);

	write(database_fd, buff, strlen(buff));
	
	write(database_fd, "\n", strlen("\n"));

	if(head == NULL){
		head = malloc(sizeof(node_t));
		memcpy(&(head->pokemon), &(pokemon[index]), sizeof(POKEMON));
	}else{
		node_t * aux = malloc(sizeof(node_t));
		memcpy(&(aux->pokemon), &(pokemon[index]), sizeof(POKEMON));
		aux->next = head;
		head = aux;
	}
	cant++;
	return 1;
}

void printList(){
	node_t * current = head;

	printf("\n------ EN ADOPCION  -------\n" );
	while(current!=NULL){
		printf("%s\n", current->pokemon.name);
		current = current->next;
	}
	printf("---------------------------\n" );
}

int print_adoptar_pokemon(POKEMON * pokemon, int id){
	if(cant == 0){
		return 0;
	}
	int aux = adoptar_pokemon(pokemon);
	stdprint(id, "adopt", cant, (*pokemon).name, (*pokemon).life);
	return aux;
}

int adoptar_pokemon(POKEMON * pokemon){

	int ganador = rand()%cant;
	
	node_t * current = head;
	node_t * previous = NULL;

	int i = 0;
	
	while(i!= ganador){
		i++;
		previous = current;
		current = current->next;
	}

	if(previous != NULL)
		previous->next = current->next;
	else
		head = current->next;
	
	cant--;
	
	memcpy(pokemon, &(current->pokemon), sizeof(POKEMON));
	free(current);

	return 1;
}

