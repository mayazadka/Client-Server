//includes
#include "clientHandler.h"

//defines
#define MAX_CLIENTS 5
#define PORT 7777
#define IP "127.0.0.1"
#define SUCCESS 0
#define CREATE_SOCKET_ERROR 1
#define BIND_SOCKET_ERROR 2
#define LISTEN_SOCKET_ERROR 3
#define ALLOCATION_ERROR 4

//structures
struct main_data{
	pthread_t tid;
	int sock;
	int connect_sock[MAX_CLIENTS];
	List* available;
	struct sockaddr_in server_name;
}typedef Main_data;

//functions
int openServer(int port, int* sock);
void closeServer(int sock);
void* runServer(void* args);
