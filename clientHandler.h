//includes
#include <sys/types.h>
#include <signal.h>
#include <mysql.h>
#include <sys/un.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "help.h"
#include "password.h"
#include "list.h"

//defines
#define MYSQLIP "127.0.0.1"
#define MYSQLUSER "root"
#define MYSQLPASSWORD "OMEome0707"
#define MYSQLDB "ottomate"
#define MAX_MESSAGE_SIZE 1024

//structures
struct specific_data{
    pthread_t tid;
    int connect_sock;
    List* available;
	int place;
}typedef Specific_data;

//functions
void* handleClient(void* args);
MYSQL_RES * queryAndResponse(int isResult, MYSQL *con, char* query);