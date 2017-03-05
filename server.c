#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>

#define MAXLINE 256
typedef struct sockaddr SA; 

struct sockaddr_in serverAddr, clientAddr;

int main(int argc, char *argv[]){
	
	int listenerFD, newSocketFD, portNO, clientLen;			
	int count, optval, bufferSize;
	char buffer[256];
	
	if (argc < 2){
		printf("Error, please include a port...\n");	//no port given at command line
		exit(1);
	}
	
	listenerFD = socket(AF_INET, SOCK_STREAM, 0);
	if (listenerFD < 0)
		perror("Error, could not open socket...\n");
	if (setsockopt(listenerFD, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int))<0)
		return -1;

	bzero((char *)&serverAddr, sizeof(serverAddr));
	portNO = atoi(argv[1]);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(portNO);

	if (bind(listenerFD, (SA*)&serverAddr, sizeof(serverAddr)) < 0)
		perror("Error binding socket...\n");
								
	listen(listenerFD, 2);					//listen on port described in
								//serverAddr, with Queue==2	
	printf("Listening on port %d\n", portNO);
	clientLen = sizeof(clientAddr);
	
	while(1){
		
		newSocketFD = accept(listenerFD, (SA*)&clientAddr, &clientLen);
		if (newSocketFD < 0){
			printf("Error accepting...\n");
			continue;
		}
		else
			printf("Client Accepted...\n");

		bzero(buffer, MAXLINE);
		printf("newsocketFD: %d, listenerFD: %d \n", newSocketFD, listenerFD);
		count = read(newSocketFD, (char*)&bufferSize ,sizeof(bufferSize));
		if (count < 0){
			printf("Error reading %s from client, count: %d...\n", buffer, count);
			continue;
		}
		else
			printf("BufferSize Received: %d...\n", bufferSize);

		count = read(newSocketFD, buffer, bufferSize);
		if (count < 0){
			printf("Error reading buffer\n");
			continue;
		}
		else
			printf("Buffer Received: %s...\n", buffer);

		count = write(newSocketFD, (char*)&bufferSize, sizeof(bufferSize));
		if (count < 0){
			printf("Error writing to client...\n");
			continue;
		}
		else
			printf("Message written to client\n");
		count = write(newSocketFD, buffer, bufferSize);
		if (count < 0){
			printf("Error writing 2 to client...\n");
		}
		else
			printf("Message written 2 to client...\n");
	}

	return 0;
}
