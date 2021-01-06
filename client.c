#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


void openClient(void* args)
{
	int sock;
	struct sockaddr_in client_name;
	int value;
	int stop = 0;
	
	printf("Client is alive and establishing socket connection. \n");
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		printf("Error opening channel");
		close(sock);
		exit(1);
	}
	
	bzero(&client_name,sizeof(client_name));
	client_name.sin_family = AF_INET;
	client_name.sin_addr.s_addr = inet_addr(ipAddress);
	client_name.sin_port = htons(port);
	
	if(connect(sock, (struct sockaddr *)&client_name, sizeof(client_name)) < 0)
	{
		printf("Error establishing communications");
		close(sock);
		exit(1);
	}
	
	printf("Connected to server.\n");
	
	
	while(!stop)
	{
		read(sock, &value, 4);
		printf("Client has received %d from socket.\n", value);
	}
	printf("Exiting now.\n");
	close(sock);
	exit(0);
}
