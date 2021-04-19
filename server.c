#include "server.h"

int stop = 0;

// opening a new server and running it in a seperted thread (using runServer method)
int openServer(int port, int* sock)
{
	pthread_t tid;
	struct sockaddr_in server_name;
	Main_data* main_data;
	
	*sock = socket(AF_INET,SOCK_STREAM,0);
	if(*sock < 0){return CREATE_SOCKET_ERROR;}

	bzero(&server_name,sizeof(server_name));
	server_name.sin_family = AF_INET;
	server_name.sin_addr.s_addr = htonl(INADDR_ANY);
	server_name.sin_port = htons(port);
	
	if(bind(*sock, (struct sockaddr *)&server_name, sizeof(server_name)) < 0)
	{
		close(*sock);
		return BIND_SOCKET_ERROR;
	}
	
	if(listen(*sock,MAX_CLIENTS) < 0)
	{
		close(*sock);
		return LISTEN_SOCKET_ERROR;
	}

	main_data = (Main_data*)malloc(sizeof(Main_data));
	if(main_data == NULL)
	{
		close(*sock);
		return ALLOCATION_ERROR;
	}
	main_data->sock = *sock;
	main_data->server_name = server_name;	

	pthread_create(&tid, NULL, runServer, main_data);
	
	return SUCCESS;
}

// listening until the server is told to stop, any time we accept a new client we handle him in a new thread (via clientHandel method)
void* runServer(void* args)
{
	pthread_t tid;
	Main_data* main_data = args;
	socklen_t len = sizeof(main_data->server_name);
	Specific_data specific_data;
	int place;

	main_data->available = initialNewList(MAX_CLIENTS);
	stop = 0;
	while(!stop)
	{
		if(main_data->connect_sock == NULL)
		{
			close(main_data->sock);
			// return error?
		}
		if(takeFirst(main_data->available, &place) == 0)
		{
			continue;
		}
		main_data->connect_sock[place] = accept(main_data->sock, (struct sockaddr *)&main_data->server_name,&len);
		if(main_data->connect_sock[place]<0)
		{
			break;
		}
		specific_data.connect_sock = main_data->connect_sock[place];
		specific_data.available = main_data->available;
		specific_data.place = place;
		pthread_create(&tid, NULL, handleClient, &specific_data);
	}

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(contains(main_data->available, i) != 1 && main_data->connect_sock[i] != -1)
		{
			write(main_data->connect_sock[i], "bye", 3);
			close(main_data->connect_sock[i]);
		}
	}

	free(main_data);
	return NULL;
}

// closing the server
void closeServer(int sock)
{
	stop = 1;
	shutdown(sock, SHUT_RDWR);
	close(sock);
}
