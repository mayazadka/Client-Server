#include "server.h"
#include "list.h"

int stop = 0;

// --------------------------------------------
// closes and relases the soceket that was open
// --------------------------------------------
void clean_up(int cond, int sock)
{
	close(sock);
	exit(cond);
}

// ---------------------------------------------------------------------------------
// opening a new server and running it in a seperted thread (using runServer method)
// ---------------------------------------------------------------------------------
int openServer(int port)
{
	int sock;
	pthread_t tid;
	struct sockaddr_in server_name;
	info_runServer* data;
	
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		exit(1);
	}

	bzero(&server_name,sizeof(server_name));
	server_name.sin_family = AF_INET;
	server_name.sin_addr.s_addr = htonl(INADDR_ANY);
	server_name.sin_port = htons(port);
	
	if(bind(sock, (struct sockaddr *)&server_name, sizeof(server_name)) < 0)
	{
		clean_up(2,sock);
	}
	
	if(listen(sock,MAX_CLIENTS) < 0)
	{
		clean_up(3, sock);
	}

	data = (info_runServer*)malloc(sizeof(info_runServer));
	if(data == NULL)
	{
		clean_up(4, sock);
	}
	data->sock = sock;
	data->server_name = server_name;	

	pthread_create(&tid, NULL, runServer, data);
	return sock;
}

// -------------------------------------------------------------------------------
// listening until the server is told to stop, any time we accept a new client we handle him
// in a new thread (via clientHandel method)
// -------------------------------------------------------------------------------
void* runServer(void* args)
{
	list available_spots = initialNewList({0,1,2,3,4,5});
	int* available;
	pthread_t tid;
	info_runServer* data = args;
	socklen_t len = sizeof(data->server_name);
	info_sock info;
	while(!stop)
	{
		take(initialNewList , available);
		if(data->connect_sock == NULL)
		{
			clean_up(5, data->sock);
		}
		data->connect_sock[available] = accept(data->sock, (struct sockaddr *)&data->server_name,&len);
		if(data->connect_sock[available]<0)
		{
			add(*available);
			break;
		}
		info.connect_sock = data->connect_sock[available];
		pthread_create(&tid, NULL, handleClient, &info);
	}
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		close(data->connect_sock[i]);
	}

	close(data->sock);
	free(data);
	return NULL;
}

// ------------------
// closing the server
// ------------------
void closeServer(int sock)
{
	stop = 1;
	close(sock);
}
