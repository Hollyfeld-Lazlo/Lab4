#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#define MAXLINE 256
typedef struct sockaddr SA;
int clientfd;			
struct hostent *hp;
struct sockaddr_in serverAdd;


int checkInput(int argc, char *argv[]){
//	printf("CHECKINPUT:: argc: %d, %s, %s, %s, \n", argc, argv[0], argv[1], argv[2]);
	if(argc < 3){
		printf("Usage: [program] [server] [port]\n");
		exit(0);
	}
	printf("CHECKINPUT\n");
}

int openClientFD(char host[], int port){




	if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	else
		printf("socket\n");
	if((hp = gethostbyname(host)) == NULL)
		return -2;
	else 
		fprintf(stderr, "gethost\n");
	bzero((char *) &serverAdd, sizeof(serverAdd));
	serverAdd.sin_family = AF_INET;
	bcopy((char *)hp->h_addr_list[0],
		(char *) &serverAdd.sin_addr.s_addr, hp->h_length);
	serverAdd.sin_port = htons(port);

	if(connect(clientfd, (SA *) &serverAdd, sizeof(serverAdd)) < 0)
		return -1;
	else
		fprintf(stderr, "connecto\n");

	return clientfd;
}




int main(int argc, char *argv[]){
	char buffer[MAXLINE];
	char *hostName;
	int count, port;
	
	checkInput(argc, argv);
	hostName = argv[1];
	port = atoi(argv[2]);
	printf("host: %s, port: %d\n", hostName, port);
	clientfd = openClientFD(hostName, port);
	if (clientfd < 0)
		fprintf(stderr, "Error getting clientfd\n");
	
	printf("\n> ");
	while(fgets(buffer, MAXLINE, stdin) != NULL){
		count = write(clientfd, buffer, MAXLINE-1);
		if (count < 0){
			fprintf(stderr, "Write Failed\n");
			exit(1);
		}
		count = read(clientfd, buffer, MAXLINE-1);   
		if (count < 0) {
			perror("Read Failed\n");
			exit(1);
		}
		//printf("\n> ");
	}
}
