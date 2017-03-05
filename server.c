#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>

#define MAXLINE 256
typedef struct sockaddr SA; 

struct sockaddr_in serverAddr, clientAddr;

int main(int argc, char *argv[]){
	
	int listenerFD, newSocketFD, portNO, clientLen;			
	int count, header;
	char buffer[256];
	
	if (argc < 2){
		printf("Error, please include a port...\n");	//no port given at command line
		exit(1);
	}
	
	listenerFD = socket(AF_INET, SOCK_STREAM, 0);
	if (listenerFD < 0)
		perror("Error, could not open socket...\n");
	
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
		if (newSocketFD < 0)
			perror("Error accepting...\n");
	
		printf("Listening on port %d\n", portNO);
		bzero(buffer, MAXLINE);
		
		count = read(newSocketFD, &header , sizeof(header));  
		if (count < 0)
			perror("Error reading from client...\n");
		
		count = read(newSocketFD, buffer, header);  
		if (count < 0)                                   		
			perror("Error reading from client...\n");


		printf("Message Received: %s...\n", buffer);
		
		header = htons(strlen(buffer));
		count = write(newSocketFD, (char*)&header, sizeof(header));
		if (count < 0)
			perror("Error writing to client...\n");
		
		count = write(newSocketFD, buffer, header);
		if (count < 0)                                  	
			perror("Error writing to client...\n"); 	
	
	}

	return 0;
}
