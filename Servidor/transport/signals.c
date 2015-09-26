#include "../../commons/com.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <semaphore.h>


const char *server_name = "/tmp/my_server";
const char *server_id = "/tmp/server_id";
char sem_name_server[30];

static pthread_mutex_t mutex;

struct sigaction act;

void fatal(char*);



void handler(int sig, siginfo_t *siginfo, void *context){

	pthread_mutex_lock(&mutex); //creo que esto no sirve para nada...

//	sleep(1); //si hago esto se caga todo :/

	printf("handling signal ______________________\n");

	pthread_mutex_unlock(&mutex);
}





void createServer(){


	pthread_mutex_init(&mutex,NULL);

	sigemptyset(&act.sa_mask);

	act.sa_sigaction = &handler;
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGUSR1, &act, NULL) < 0) {
		perror ("sigaction SIGUSR1");
	}


	sprintf(sem_name_server,"/sem");
	sem_t* sem = sem_open(sem_name_server,O_CREAT,0777,0);
	if(sem == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}

	int listener;

	if((listener = open(server_name, O_RDONLY|O_CREAT, 0666)) == -1){
		fatal("Error open listener");
	}

	close(listener); 


	int server_id_fd;
	if((server_id_fd = open(server_id, O_WRONLY|O_CREAT, 0666)) == -1){
		fatal("Error open listener");
	}
	char id_buff[10];

	sprintf(id_buff, "%d", getpid());

	printf("buff : %s\n", id_buff);

	int n;
	if((n = write(server_id_fd, id_buff, strlen(id_buff))) == -1){
		fatal("Error writing in server_id_fd");
	}


	sem_post(sem);
}

void acceptConnection(CONNECTION *c){

	printf("BEGINNING CONNECTION\n");

	sigset_t signal_set;
	int sig;


	sigemptyset (&signal_set);
	sigaddset (&signal_set, SIGUSR1);


	sigwait(&signal_set, &sig);


	int listener = open(server_name, O_RDONLY);

	char buff[30];

	int n;
	int i = 0;
		do{
			n = read(listener, buff + i, 1);
			if(n){
				i++;
			}
		}while(n > 0);


	c->pid = atoi(buff);

	printf("El pid del cliente es c-> pid : %d\n", c->pid);


	char f_S[35], f_R[35];

	printf("connecting to %d\n",c->pid);

	sprintf(f_R,"/tmp/file_clsv-%d",c->pid);
	printf("%s\n",f_R);
	sprintf(f_S,"/tmp/file_svcl-%d",c->pid);
	printf("%s\n",f_S);




	int fd_svcl;

	if ( (fd_svcl = open(f_S, O_WRONLY|O_CREAT, 0666)) == -1 )
		fatal("Error open f_S");


	printf("fd_svcl : %d\n", fd_svcl);

	close(fd_svcl);

	

	int fd_clsv;

	if ( (fd_clsv = open(f_R, O_RDONLY|O_CREAT, 0666)) == -1 )
		fatal("Error open f_R");

	close(fd_clsv);

	kill(atoi(buff), SIGUSR1);

	close(listener);


}


int receivePacket(CONNECTION* c, PACKET *p, int size){


	char f_R[35];

	printf("receiving fromd %d\n",c->pid);


        sigset_t signal_set;
        int sig;


	sigemptyset (&signal_set);
	sigaddset (&signal_set, SIGUSR1);


//	sigwait(&signal_set, &sig);




	sprintf(f_R,"/tmp/file_clsv-%d",c->pid);
	printf("%s\n",f_R);

	int fd_clsv;

	if ( (fd_clsv = open(f_R, O_RDONLY)) == -1 )
		fatal("Error open f_R");

	char buff[sizeof (PACKET)];


	int n;

		n = read(fd_clsv, buff, sizeof (PACKET));

	memcpy(p, buff, sizeof (PACKET));


	return n;
}


int sendPacket(CONNECTION *c, PACKET *p,int size){

	char buff[sizeof (PACKET)];

	memcpy(buff, p, sizeof (PACKET));

	char f_S[35];

	sprintf(f_S,"/tmp/file_svcl-%d",c->pid);
	printf("%s\n",f_S);

	int fd_svcl;

	if ( (fd_svcl = open(f_S, O_WRONLY)) == -1 )
		fatal("Error open f_S");

	int n = write(fd_svcl, buff, sizeof (PACKET));

	char pid[30];
	sprintf(pid, "%d", c->pid);

	kill(atoi(pid), SIGUSR1);


	return n;
}





int endConnection(CONNECTION *c){
	char f_S[35], f_R[35];

	printf("ending connection\n");

	sprintf(f_R,"/tmp/file_clsv-%d",c->pid);
	printf("voy a matar a %s\n",f_R);
	sprintf(f_S,"/tmp/file_svcl-%d",c->pid);
	printf("voy a matar a %s\n",f_S);

	remove(f_R);
	remove(f_S);
	return 1;
}

void killServer(int signo){
	printf("killing server\n");
	remove(sem_name_server);
	remove(server_name);
	remove(server_id);
	exit(1);

}



void
fatal(char *s)
{
	remove(sem_name_server);
	remove(server_name);
	remove(server_id);
	perror(s);
	exit(1);
}
