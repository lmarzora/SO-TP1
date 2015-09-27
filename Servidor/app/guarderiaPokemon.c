#include "../../commons/myTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>

typedef struct node
{
	POKEMON pokemon;
	struct node * next;
} node_t;

node_t * head;

int cant;


int add(POKEMON pokemon){

	if(head == NULL){
		head = malloc(sizeof(node_t));
		memcpy(&(head->pokemon), &pokemon, sizeof(POKEMON));
	}else{
		node_t * aux = malloc(sizeof(node_t));
		memcpy(&(aux->pokemon), &pokemon, sizeof(POKEMON));
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


int pokemon_adopt(POKEMON * pokemon){
	

	if(cant == 0){
		return 0;
	}

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

