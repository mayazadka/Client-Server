#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "help.h"
#include "password.h"
#include "list.h"
#include "Error.h"

#ifdef __linux__            // UNIX/Linux
#include <netinet/in.h>
#include <mysql.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYSQLIP "127.0.0.1"
#define MYSQLUSER "root"
#define MYSQLPASSWORD "OMEome0707"
#define MYSQLDB "ottomate"

#endif
struct specific_data{
    pthread_t tid;
    int connect_sock;
    List* available;
	int place;
}typedef Specific_data;

int handleOBDclient(MYSQL * con, int client, char* car_id, char* customer_id, char* password);
void handleStatisticInformationManager(MYSQL * con, int client, char* drive_id, char* username, char* password);
void handleWorker(MYSQL * con, int client, char* worker_id, char* password);
int handleClient(void* args);
