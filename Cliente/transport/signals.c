#include "../../commons/com.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>



const char *server_name = "/tmp/server_id";
const char *server = "/tmp/my_server";
static char semName[30];
static char clsv[35];
static char svcl[35];
char server_id[30];





struct sigaction act;

struct sigaction act2;

void handler(int sig);

int requestConnection(CONNECTION* c) {

	sigset_t signal_set;
	int sig;

	sigemptyset (&signal_set);
	sigaddset (&signal_set, SIGUSR1);
	sigaddset (&signal_set, SIGUSR2);


	sigprocmask(SIG_BLOCK, &signal_set, NULL);

	signal(SIGUSR1, &handler);
	signal(SIGUSR2, &handler);


	sprintf(semName,"/sem");
	sem_t* sem = sem_open(semName,O_CREAT,0777, 0);
	if(sem == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}
	

	int n;

	int server_id_fd;
	if((server_id_fd = open(server_name, O_RDONLY)) == -1){
		perror("fail");
	}


	int i = 0;
	do{
		n = read(server_id_fd, server_id + i, 1);
		i ++;
	}while(n > 0);


	close(server_id_fd);

	//printf("server id : %d\n", atoi(server_id));

	int server_fd;
	sem_wait(sem);
	if((server_fd = open(server, O_WRONLY|O_APPEND)) == -1){
		perror("fail");
	}

	char buff[30];
	sprintf(buff, "%d\n", getpid());

	//printf("my pid is : %s\n", buff);


	if((n = write(server_fd, buff, strlen(buff))) == -1){
		perror("fail writing in /tmp/my_server");
	}

	close(server_fd);

	kill(atoi(server_id), SIGUSR1);

	//printf("mande la señal\n");




	sprintf(clsv,"/tmp/file_clsv-%d",getpid());
	sprintf(svcl,"/tmp/file_svcl-%d",getpid());


	sigset_t signal_set2;
	int sig2;
	sigaddset (&signal_set2, SIGUSR1);
	
	sigwait(&signal_set2, &sig2);


	return 1;
}

int sendPacket(CONNECTION* c, PACKET* p,int size){
	//printf("sending packet\n");



	int f_S = open(clsv, O_WRONLY);
	if(f_S < 0 )
	{
		perror("error");
		exit(1);	
	}

	char buff[sizeof (PACKET)];

	memcpy(buff, p, sizeof (PACKET));

	int n = write(f_S, buff, sizeof (PACKET));
	if(n < 0){
		perror("ERROR writing to file");
		exit(1);
	}
	close(f_S);
	//kill(atoi(server_id), SIGUSR1);
	kill(atoi(server_id), SIGUSR2);
	return n;
}


void handler(int sig){
	
	//printf("signal : %d\n",sig);
	return;

}

int receivePacket(CONNECTION* c, PACKET* p,int size){

	//printf("receiving packet\n");



//	while(flag_signal);

	sigset_t signal_set;
	int sig;
	sigaddset (&signal_set, SIGUSR2);
	
	sigwait(&signal_set, &sig);

	int f_R;

	if((f_R = open(svcl, O_RDONLY)) == -1){
		perror("Error open f_R");
	}
	if(f_R < 0 )
	{
		perror("error");
		exit(1);
	}


	char buff[sizeof (PACKET)];

	int n = read(f_R, buff, sizeof (PACKET));

	

	if (n < 0)
  	{
     		perror("ERROR reading from file");
 		exit(1);
 	}

	memcpy(p, buff, sizeof (PACKET));
	close(f_R);


	return n;
}


int endConnection(CONNECTION* c){
	remove(semName);
	remove(clsv);
	remove(svcl);
	return 1;
}
