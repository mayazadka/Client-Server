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
#endif
#define MAX_CLIENTS 5
#include "clientHandler.h"

struct info_server{
    pthread_t tid;
    int port;
}typedef info_server;

struct info_sock{
    pthread_t tid;
    int connect_sock;
}typedef info_sock;

struct info_runServer{
	pthread_t tid;
	int sock;
	int* connect_sock;
	struct sockaddr_in* server_name;
}typedef info_runServer;
void* runServer(void* args);
void clean_up(int cond, int *sock);
void* openServer(void* args);
void stopServer();
