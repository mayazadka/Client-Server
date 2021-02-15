#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#ifdef __linux__            // UNIX/Linux
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#define MAX_CLIENTS 5
#define PORTS 2021
#define PORTC 2021
#define IP "127.0.0.1"

#include "clientHandler.h"

struct info_sock{
    pthread_t tid;
    int connect_sock;
}typedef info_sock;

struct info_runServer{
	pthread_t tid;
	int sock;
	int connect_sock[MAX_CLIENTS];
	struct sockaddr_in server_name;
}typedef info_runServer;

int openServer(int port);
void closeServer(int sock);

void* runServer(void* args);
void clean_up(int cond, int sock);

int openClient(char* ip, int port);
void closeClient(int sock);
