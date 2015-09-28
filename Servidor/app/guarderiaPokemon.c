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

void last_print();

static char *database_name = "/tmp/database";

int cant = 0;

int print_regalar_pokemon(POKEMON * pokemon, int index, int id){

	int aux = regalar_pokemon(pokemon, index);
	stdprint(id, "abandon", cant, pokemon[index].name, pokemon[index].life);
	return aux;

}
int regalar_pokemon(POKEMON * pokemon, int index){


	printf("aceptando un nuevo pokemon...\n");
	sleep(5);

	int database_fd;
	if((database_fd = open(database_name, O_WRONLY|O_APPEND|O_CREAT, 0666)) == -1){
		perror("fail opening database");
	}

	write(database_fd, pokemon[index].name, strlen(pokemon[index].name));
	write(database_fd, ";", strlen("\n"));

	char buff[30];
	sprintf(buff,"%d",pokemon[index].life);



	write(database_fd, buff, strlen(buff));
	
	write(database_fd, "\n", strlen("\n"));
/*
	if(head == NULL){
		head = malloc(sizeof(node_t));
		memcpy(&(head->pokemon), &(pokemon[index]), sizeof(POKEMON));
	}else{
		node_t * aux = malloc(sizeof(node_t));
		memcpy(&(aux->pokemon), &(pokemon[index]), sizeof(POKEMON));
		aux->next = head;
		head = aux;
	}*/
	cant++;
	return 1;
}

void printList(){
//	node_t * current = head;

	printf("\n------ EN ADOPCION  -------\n" );
/*	while(current!=NULL){
		printf("%s\n", current->pokemon.name);
		current = current->next;
	}*/
	int database_fd;
	if((database_fd = open(database_name, O_RDONLY)) == -1){
		perror("fail opening database");
	}

	char name[50];
	char buff[2];
	buff[0] = 0;
	buff[1] = 0;
	int i = 0;

	do{
		if(read(database_fd, buff, 1)==0){
			last_print();
			return;
		}
		name[i] = buff[0];
	}while(strcmp(buff,"\n") != 0);
	name[--i] = 0;


}

void last_print(){
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

	int other_fd;
	int database_fd;

	int ganador = rand()%cant;

	if((database_fd = open(database_name, O_RDWR)) == -1){
		perror("fail opening database");
	}

	if((other_fd = open(database_name, O_RDWR)) == -1){
		perror("fail opening database");
	}

	int another_fd;

	if((another_fd = open(database_name, O_RDWR)) == -1){
		perror("fail opening database");
	}


	char buff[2];
	buff[1] = 0;
	char answer[50];
	char life[30];
	char buff2[2];

	int n = 0;

	printf("ganador : %d\n", ganador);

	if(!is_valid_line(&another_fd)){
		ganador++;
		printf("aumento ganador %d\n", ganador);
	}
	while(n < ganador){

		read(other_fd, buff2, 1);
		read(database_fd, buff, 1);
		if(strcmp(buff, "\n") == 0){
			printf("llegue a new line\n");
			n++;
			if(!is_valid_line(&another_fd)){
				ganador++;
				printf("aumento ganador %d\n", ganador);
			}
		}		
	}

	buff[0] = 0;
	buff[1] = 0;


	int i = 0;
	do{
		read(database_fd, buff, 1);
		answer[i++] = buff[0];
	}while(strcmp(buff, ";") != 0);
	answer[--i] = 0;

	printf("answer : %s\n",answer);

	strcpy(pokemon->name, answer);

	i = 0;

	do{
		read(database_fd, buff, 1);
			life[i++] = buff[0];
	}while(strcmp(buff, "\n") != 0);
	life[--i] = 0;

	pokemon->life = atoi(life);

	write(other_fd, "&", 1);

	cant--;


/*	
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
*/
	return 1;
}


int is_valid_line(int *fd){

	char buff[2];


	buff[0] = 'l';
	buff[1] = 0;

	int flag = 1;
	while(strcmp(buff,"\n") != 0){
		read(*fd, buff, 1);
		if(!strcmp(buff, "&")){
			("no vale\n");
			flag = 0;
		}
	}

	return flag;

}



