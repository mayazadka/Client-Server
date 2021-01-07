#include "server.h"

int stop = 0;
struct info_run_server{
	pthread_t tid;
	int sock;
	int* connect_sock;
	struct sockaddr_in* server_name;

}typedef info_run_server;

void clean_up(int cond, int *sock)
{
	printf("Exiting now.\n");
	close(*sock);
	exit(cond);
}

int openServer(void* args)
{
	pthread_t tid;
	int sock;
	int* connect_sock = (int*)malloc(MAX_CLIENTS * sizeof(int));
	struct sockaddr_in server_name;
	
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		printf("Error opening channel");
		clean_up(1,&sock);
	}
	info_server* info = args;
	bzero(&server_name,sizeof(server_name));
	server_name.sin_family = AF_INET;
	server_name.sin_addr.s_addr = htonl(INADDR_ANY);
	server_name.sin_port = htons(info->port);
	
	if(bind(sock, (struct sockaddr *)&server_name, sizeof(server_name)) < 0)
	{
		printf("Error naming channel");
		clean_up(1,&sock);
	}
	printf("Server is alive and waiting for socket connection from client.\n");
	listen(sock,MAX_CLIENTS);

	info_run_server* data;
	data->sock = sock;
	data->connect_sock = connect_sock;
	data->server_name = &server_name;	
	pthread_create(&tid, NULL, runServer, data);
	return sock;
}
void* runServer(void* args)
{
	int i = 0;
	pthread_t tid;
	info_run_server* data = args;
	size_t len = (size_t) sizeof(data->server_name);
	info_sock* info;

	while(!stop)
	{
		data->connect_sock[i] = accept(data->sock, (struct sockaddr *)&data->server_name, &len);
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