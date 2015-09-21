//fifos.c
#include "../../commons/com/clserv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

static int pid, n;

static int fdSvCl;
static int fdClSv;

static char *pipeSvCl = "/tmp/pipeSvCl";
static char *pipeClSv = "/tmp/pipeClSv";


int connect_to_server() {
	if((fdSvCl = open(pipeSvCl, O_RDONLY)) == -1)
		perror("ERROR connecting");

	if((fdClSv = open(pipeClSv, O_WRONLY)) == -1)
		perror("ERROR connecting");

	return 1;
}


int send_packet(void* p, int size ){
	printf("sending packet\n");
	n = write(fdClSv, p, size);
	if(n < 0){
		perror("ERROR writing to fifo");
		exit(1);
	}
	return n;
}

int receive_packet(void* p, int size ){
	printf("receiving packet\n");
	n = read(fdSvCl, p,size);
	if (n < 0)
   {
      perror("ERROR reading from fifo");
      exit(1);
   }

	return n;
}


void close_server(){
	//close(fdClSv); //si hago esto el servidor muere despues de curar
	close(fdSvCl);
	return;
}



