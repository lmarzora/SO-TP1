#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include "../../commons/com/clserv.h"



int creatServ()
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
	return sockfd;
}

int receive_packet( void *p, int lim, int sockfd ) {
	
	printf("receiving packet\n");
	int clilen, newsockfd;
	struct sockaddr_in cli_addr;
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
	{
		perror("ERROR on accept");
		exit(1);
	}
	int qty = read(newsockfd,p,lim);
	printf("packet received\n");
	printf("qty = %d\n",qty);
	return newsockfd;
}

int send_packet( void*p , int qty , int cli) {
	printf("sending packet\n");
	int n = write(cli,p,qty);
	close(cli);
	printf(" n = %d\n" , n);	
	return n;
}















