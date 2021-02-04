#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

#include "clientHandler.h"
#include "server.h"
int stop = 0;

// ------------------------------------------------------------------
// closes and relases the soceket that was opwn for a sesipic client
// ------------------------------------------------------------------
void clean_up(int cond, int *sock)
{
	printf("Exiting now.\n");
	close(*sock);
	exit(cond);
}
// ---------------------------------------------------------------------------------
// opening a new server and running it in a seperted thread( using runServer method)
// ---------------------------------------------------------------------------------

void* openServer(int port)
{
	pthread_t tid;
	int* sock = malloc(sizeof(int));
	struct sockaddr_in server_name;
	info_runServer* data = malloc(sizeof(info_runServer));
	
	*sock = socket(AF_INET,SOCK_STREAM,0);
	if(*sock < 0)
	{
		printf("Error opening channel\n");
		clean_up(1,sock);
	}
	printf("openServer seccess\n");
	bzero(&server_name,sizeof(server_name));//
	server_name.sin_family = AF_INET;
	server_name.sin_addr.s_addr = htonl(INADDR_ANY);
	server_name.sin_port = htons(port);
	
	if(bind(*sock, (struct sockaddr *)&server_name, sizeof(server_name)) < 0)
	{
		printf("Error naming channel\n");
		clean_up(1,sock);
	}
	
	printf("Server is alive and waiting for socket connection from client %d.\n" , *sock);
	
	if(listen(*sock,MAX_CLIENTS) < 0){printf("exit from listen\n"); return NULL;}
	data->sock = *sock;
	data->server_name = &server_name;	
	pthread_create(&tid, NULL, runServer, data);
	pthread_join(tid ,NULL);
	return sock;
}

// --------------------------------------------------------------------------
// listing until told to stop , any time we accept a new client we handle him,
// i a new thread (via clientHandel method)
// --------------------------------------------------------------------------
void* runServer(void* args)
{
	int i = 0;
	pthread_t tid;
	info_runServer* data = args;
	socklen_t len = sizeof(data->server_name);
	info_sock* info = malloc(sizeof(info_sock));
	while(!stop)
	{
		data->connect_sock = malloc(len * sizeof(int));
		data->connect_sock[i] = accept(data->sock, (struct sockaddr *)&data->server_name,&len);
		info->connect_sock = data->connect_sock[i];
		pthread_create(&tid, NULL, handleClient, info);
		i++;
	}
	for(int i = 0; i < MAX_CLIENTS; i++)
		close(data->connect_sock[i]);
	clean_up(0,&data->sock);
	return 0;
}
void stopServer()
{
	stop = 1;
}
int connectionWithMySQLServer()
{
	MYSQL *con = mysql_init(NULL);
	if (con == NULL)
	{
		puts(mysql_error(con));
		return 1;
	}
	if(mysql_real_connect(con, "127.0.0.1", "root", "529123",  "ottomate", 0, NULL, 0) == NULL)
	{
		puts(mysql_error(con));
		mysql_close(con);
		return 1;
	}
	//"INSERT INTO car (car_id, manufacturing_year, model) VALUES('123456',2020,'JAGUAR F-PACE');"
	
	mysql_close(con);
	return 0;
}
void UpdateDB(MYSQL *con, char* query)
{
	if(mysql_query(con, query))
	{
		puts(mysql_error(con));
		mysql_close(con);
		return 1;
	}
}
void GetFromDB(MYSQL *con)
{
	if(mysql_query(con, "SELECT * FROM car"))
	{
		puts(mysql_error(con));
		mysql_close(con);
		return 1;
	}
	MYSQL_RES *result =  mysql_store_result(con);
	if(result == NULL)
	{
		puts(mysql_error(con));
		mysql_close(con);
		return 1;
	}
	int num_fields = mysql_num_fields(result);
	MYSQL_ROW row;
	while((row = mysql_fetch_row(result)))
	{
		for(int i = 0; i < num_fields; i++)
		{
		printf("%s ", row[i]?row[i]:"NULL");
		}
		printf("\n");
	}

	mysql_free_result(result);
}