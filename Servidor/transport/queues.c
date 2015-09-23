#include "../../commons/com.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <semaphore.h>




const char *server_name = "/my_server";
static mqd_t q_listener;
static mqd_t q_clsv;
static mqd_t q_svcl;
static char q_S[35], q_R[35];



struct mq_attr q_clsv_attr;
struct mq_attr q_svcl_attr;



char semName[35];

void fatal(char*);

void createServer(){
	struct mq_attr listener_attr;
//	listener_attr.mq_flags = 0;
	listener_attr.mq_maxmsg = 10;
	listener_attr.mq_msgsize = sizeof (int);
//	listener_attr.mq_curmsgs = 0;


	mq_unlink(server_name);

if ( (q_listener = mq_open(server_name, O_RDONLY|O_CREAT, 0666, &listener_attr)) == -1 )
	fatal("Error mq_open q_listener");

	mq_close(q_listener);
	printf("server born\n");

	return;
}

int sendPacket(CONNECTION *c, PACKET *p,int size){
	printf("sending packet\n");
	int n;


	sprintf(q_S,"/queue_svcl-%d",c->pid);
	printf("%s\n",q_S);

	static mqd_t q_svcl;


	if(q_svcl = mq_open(q_S, O_WRONLY) == -1){
		fatal("Error mq_open q_svcl");
	}

	n = mq_send(q_svcl, (const char *)p,size, 0);
	mq_close(q_svcl);
	return n;
}


int receivePacket(CONNECTION* c, PACKET *p, int size){
	int qty;

	printf("espero paquete\n");

	sprintf(q_R,"/queue_clsv-%d",c->pid);
	printf("%s\n",q_R);

	mqd_t q_clsv;

	if((q_clsv = mq_open(q_R, O_RDONLY)) == -1){
		fatal("Error mq_open q_clsv");
	}

	printf("abri\n");

	qty = mq_receive(q_clsv, (char *)p,sizeof(PACKET), NULL);
	printf("recibi\n");
	mq_close(q_svcl);


	printf("packet received\n");
	return qty;
}

int endConnection(CONNECTION *c){
	return 1;
}

void acceptConnection(CONNECTION *c){
	q_listener = mq_open(server_name, O_RDONLY);

	int n;
	
	n = mq_receive(q_listener, (char *)&(c->pid), sizeof(int), 0);
	if(n<=0){
		perror("error");
	}

	printf("pid = %d\n", c->pid);


	printf("connecting to %d\n",c->pid);

	sprintf(q_R,"/queue_clsv-%d",c->pid);
	printf("%s\n",q_R);
	sprintf(q_S,"/queue_svcl-%d",c->pid);
	printf("%s\n",q_S);


	sprintf(semName,"/sem-%d",c->pid);
	printf("%s\n",semName);
	sem_t* sem = sem_open(semName,O_CREAT,0777,0);
	if(sem == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}

	q_svcl_attr.mq_maxmsg = 1;
	q_svcl_attr.mq_msgsize = sizeof (PACKET);


	if ( (q_svcl = mq_open(q_S, O_WRONLY|O_CREAT, 0666, &q_svcl_attr)) == -1 )
		fatal("Error mq_open q_S");
	mq_close(q_svcl);

	

	q_clsv_attr.mq_maxmsg = 1;
	q_clsv_attr.mq_msgsize = sizeof (PACKET);


	if ( (q_clsv = mq_open(q_R, O_RDONLY|O_CREAT, 0666, &q_clsv_attr)) == -1 )
		fatal("Error mq_open q_R");
	mq_close(q_clsv);

	sem_post(sem);
	sem_close(sem);
	mq_close(q_listener);


	printf("termino acceptConnection\n");

}


void killServer(int signo){
	printf("killing server\n");
	remove(semName);
	mq_unlink(server_name);
	exit(1);

}

void
fatal(char *s)
{
	remove(semName);
	perror(s);
	exit(1);
}



