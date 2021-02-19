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
#define PORTS 7777
#define IP "127.0.0.1"

#include "clientHandler.h"

struct main_data{
	pthread_t tid;
	int sock;
	int connect_sock[MAX_CLIENTS];
	List* available;
	struct sockaddr_in server_name;
}typedef Main_data;

int openServer(int port);
void closeServer(int sock);

void* runServer(void* args);
void clean_up(int cond, int sock);
