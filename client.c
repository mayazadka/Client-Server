#include "client.h"

int openClient(void* args)
{
	int sock;
	struct sockaddr_in client_name;
	
	printf("Client is alive and establishing socket connection. \n");
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		printf("Error opening channel");
		close(sock);
		exit(1);
	}
	info_client* info = args;
	bzero(&client_name,sizeof(client_name));
	client_name.sin_family = AF_INET;
	client_name.sin_addr.s_addr = inet_addr(info->ip);
	client_name.sin_port = htons(info->port);
	
	if(connect(sock, (struct sockaddr *)&client_name, sizeof(client_name)) < 0)
	{
		printf("Error establishing communications");
		close(sock);
		exit(1);
	}
	printf("Connected to server.\n");
	return sock;
}
void closeClient(int sock) {
	printf("Closing client %d channel\n", sock);
	close(sock);
	exit(0);
}