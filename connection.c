#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread>

#include "client.h"
#include "server.h"

#define PORT 1234

void CreateNewClient();
void openServer();
void closeServer();
void send();
void receive();

int CreateNewClient() {
	char ipAddress[16];
	int port;
	pthread_t tid:
	int err;
	
	printf("Please enter ip address and port: ");
	scanf("%s", ipAddress);
	scanf("%d", &port);
	printf("%s, %d, ", ipAddress, &port);
	void* args[] = {&tid, &ipAddress, &port};
	err = pthread_creat(&tid, NULL, openClient, *args);
	if(err != 0)
	{
		printf("error with creating thread\n");
		exit(1);
	}
	return tid;
}
int openServer() {
	pthread_t tid;
	void* args[] = {&tid, &PORT};
	pthread_creat(&tid, NULL, openServer, args);
	return tid;
}
void closeServer(int tid) {
	
}
void send();
void receive();

