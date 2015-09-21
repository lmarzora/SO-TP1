//fifos.c
#include "../../commons/com/clsv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

static char *pipeClSv;
static char *pipeSvCl;

static int fdSvCl;
static int fdClSv;


void fatal(char*);
void keep_waiting(int s);


void createServer(){

	printf("creating serv\n");
	pipeClSv = "/tmp/pipeClSv";
	if ( access(pipeClSv, 0) == -1 && mknod(pipeClSv, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod pipeClSv");
	//fdClSv = open(pipeClSv, O_RDONLY);
	pipeSvCl = "/tmp/pipeSvCl";
	if ( access(pipeSvCl, 0) == -1 && mknod(pipeSvCl, S_IFIFO|0666, 0) == -1 )
		fatal("Error mknod pipeSvCl");
	//fdSvCl = open(pipeSvCl, O_WRONLY);

	printf("server born\n");



	if(signal(SIGPIPE,killServer)== SIG_ERR)
		printf("error catching sigpipe\n");
	return;
}


int sendPacket(CONNECTION *c, PACKET *p, int lim){
	printf("sending packet\n");
	int n;
	n = write(c->pipes[0],p,lim);
	return n;
}

int receivePacket(CONNECTION* c, PACKET *p, int lim){
	int qty;
	qty = read(c->pipes[1],p,lim);

//	if(signal(SIGPIPE,SIG_IGN)== SIG_ERR)
//		printf("error catching sigpipe\n");




	printf("packet received\n");
	return qty;
}


int closeConnection(CONNECTION *c){
	close(c->pipes[0]);
	close(c->pipes[1]);
	printf("pipes closed\n");
	return 1;
}

void acceptConnection(CONNECTION *c){
	c->pipes[0] = open(pipeSvCl, O_WRONLY);
	c->pipes[1] = open(pipeClSv, O_RDONLY);

	fdSvCl = c->pipes[0];
	fdClSv = c->pipes[1];
	return;
}

void keep_waiting(int s){
	printf("waiting for trainers\n");
}


void killServer(int signo)
{
	printf("killing server\n");
	close(fdSvCl);
	close(fdClSv);
	exit(1);
}

