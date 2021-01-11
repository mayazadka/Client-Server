#include "server.h"
int stop = 0;

//---------------------------------------
//closes and relases the soceket that was opwn for a sesipic client
//-------------------------------------
void clean_up(int cond, int *sock)
{
	printf("Exiting now.\n");
	close(*sock);
	exit(cond);
}

void* openServer(void* args)
{
	pthread_t tid;
	int* sock = malloc(sizeof(int));
	struct sockaddr_in server_name;
	int* connect_sock = (int*)malloc(MAX_CLIENTS * sizeof(int));
	info_server* info = args;
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
	server_name.sin_port = htons(info->port);
	
	if(bind(*sock, (struct sockaddr *)&server_name, sizeof(server_name)) < 0)
	{
		printf("Error naming channel\n");
		clean_up(1,sock);
	}
	
	printf("Server is alive and waiting for socket connection from client.\n");
	
	if(listen(*sock,MAX_CLIENTS) < 0){printf("exit from listen\n"); return -1;}
	data->sock = *sock;
	data->connect_sock = connect_sock;
	data->server_name = &server_name;	
	pthread_create(&tid, NULL, runServer, data);
	pthread_join(tid ,NULL);
	return sock;
}
//
//
//
void* runServer(void* args)
{
	int i = 0;
	printf("bf tid\n");
	pthread_t tid;
	printf("bf data = args\n");
	info_runServer* data = args;
	printf("bf len = sizeOf\n");
	socklen_t len = sizeof(data->server_name);
	printf("bf infp = malloc\n");
	info_sock* info = malloc(sizeof(info_sock));
	printf("starts run\n");
	for(int j = 0 ; j < 10 ; j++)
	{
		printf("in while\n");
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