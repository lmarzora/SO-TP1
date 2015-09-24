#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include<unistd.h>
#include "../../commons/com.h"
#include<signal.h>

static int serv;

void createServer()
{

	printf("creating serv\n");
	int sockfd, portno;
	char buffer[256];
	struct sockaddr_in serv_addr;
	
   
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
	if (sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}
   
/* Initialize socket structure */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 5001;
   
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
   
/* Now bind the host address using bind() call.*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("ERROR on binding");
		exit(1);
	}
	listen(sockfd,5);
	printf("server born\n");
	serv = sockfd;
	

	return ;
}




void acceptConnection(CONNECTION* c)
{
	printf("listening\n");
	int sockfd = serv;
	int clilen;
	struct sockaddr_in cli_addr;
	clilen = sizeof(cli_addr);

	c->sockfd = accept(serv, (struct sockaddr *) &cli_addr, &clilen);
	if (c->sockfd < 0)
	{
		perror("ERROR on accept");
		exit(1);
	}


}

int receivePacket(CONNECTION* c, PACKET *p, int size) {
	int qty = read(c->sockfd,p,size);
	printf("packet received\n");
	return qty;
}

int sendPacket(CONNECTION* c, PACKET *p, int size) {
	printf("sending packet\n");
	int n = write(c->sockfd,p,size);
	return n;
}

int endConnection(CONNECTION* c)
{
	close(c->sockfd);
	printf("socket closed\n");
	return 1;
}

void killServer(int signo)
{
	printf("killing server\n");
	shutdown(serv,2);
}












