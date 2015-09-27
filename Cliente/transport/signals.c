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
static flag_signal;
static char clsv[35];
static char svcl[35];
char server_id[30];

struct sigaction act;

void handler(int sig, siginfo_t *siginfo, void *context);

int requestConnection(CONNECTION* c) {

	sigemptyset(&act.sa_mask);

	act.sa_sigaction = &handler;
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGUSR1, &act, NULL) < 0) {
		perror ("sigaction SIGUSR1");
	}




	sprintf(semName,"/sem");
	sem_t* sem = sem_open(semName,O_CREAT,0777, 0);
	if(sem == SEM_FAILED)
	{
		perror("fail");
		exit(1);
	}
	sem_wait(sem);


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

	printf("server id : %d\n", atoi(server_id));

	int server_fd;
	if((server_fd = open(server, O_WRONLY)) == -1){
		perror("fail");
	}

	char buff[30];
	sprintf(buff, "%d", getpid());

	printf("my pid is : %s\n", buff);


	if((n = write(server_fd, buff, strlen(buff))) == -1){
		perror("fail writing in /tmp/my_server");
	}

	close(server_fd);

	kill(atoi(server_id), SIGUSR1);


	flag_signal = 1;


	sprintf(clsv,"/tmp/file_clsv-%d",getpid());
	sprintf(svcl,"/tmp/file_svcl-%d",getpid());
	sem_post(sem);

	while(flag_signal);


	return 1;
}

int sendPacket(CONNECTION* c, PACKET* p,int size){
	printf("sending packet\n");



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

	kill(atoi(server_id), SIGUSR1);

	printf("mande la señal\n");

	return n;
}


void handler(int sig, siginfo_t *siginfo, void *context){

	flag_signal = 0;

}

int receivePacket(CONNECTION* c, PACKET* p,int size){

	printf("receiving packet\n");

	flag_signal = 1;

	while(flag_signal);

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
	//remove(clsv);
	//remove(svcl);
	return 1;
}
