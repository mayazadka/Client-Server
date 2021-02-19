#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "help.h"
#include "password.h"
#include "list.h"
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

void* handleClient(void* args);
MYSQL_RES * queryAndResponse(int isResult, MYSQL *con, char* query);