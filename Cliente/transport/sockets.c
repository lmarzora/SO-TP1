#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include "../../commons/com/clserv.h"

int send_packet(void* p, int size, int serv)
{	
	printf("sending packet\n");
	int sockfd = serv;
	int n = write(sockfd,p,size);
	if (n < 0)
   {
      perror("ERROR writing to socket");
      exit(1);
   }
	return n;
}

int recive_packet(void* p, int size,int serv)
{
	printf("receiving packet\n");
	int sockfd = serv;
	int n = read(sockfd, p,size);
	if (n < 0)
   {
      perror("ERROR reading from socket");
      exit(1);
   }

	return 1;

}


int connect_to(char* hostname, char* port) {
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
   

   	portno = atoi(port);
   
   /* Create a socket point */
  	 sockfd = socket(AF_INET, SOCK_STREAM, 0);
  	 
  	 if (sockfd < 0)
  	 {
  	    perror("ERROR opening socket");
  	    exit(1);
  	 }
	 server = gethostbyname(hostname);
   
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	   
	   bzero((char *) &serv_addr, sizeof(serv_addr));
	   serv_addr.sin_family = AF_INET;
	   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	   serv_addr.sin_port = htons(portno);
	   
	   /* Now connect to the server */
	   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	   {
		  perror("ERROR connecting");
		  exit(1);
	   }
	return sockfd;
}































