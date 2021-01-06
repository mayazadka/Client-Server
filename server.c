#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MAX_CLIENTS 5

int stop = 0;

void clean_up(int cond, int *sock)
{
	printf("Exiting now.\n");
	close(*sock);
	exit(cond);
}

void openServer(void* vars)
{
	int sock;
	int* connect_sock = (int*)malloc(MAX_CLIENTS * sizeof(int));
	struct sockaddr_in server_name;
	size_t len;
	int i = 0;
	
	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		printf("Error opening channel");
		clean_up(1,&sock);
	}
	
	bzero(&server_name,sizeof(server_name));
	server_name.sin_family = AF_INET;
	server_name.sin_addr.s_addr = htonl(INADDR_ANY);
	server_name.sin_port = htons(*vars[1]);
	
	if(bind(sock, (struct sockaddr *)&server_name, sizeof(server_name)) < 0)
	{
		printf("Error naming channel");
		clean_up(1,&sock);
	}
	printf("Server is alive and waiting for socket connection from client.\n");
	listen(sock,1);
	
	len = sizeof(server_name);

	while(!stop)
	{
		void* args1[] = {&tid, &connect_sock[i]};
		connect_sock[i] = accept(sock, (struct sockaddr *)&server_name, &len);
		pthread_creat(&tid, NULL, openServer, *args1);
		i++;
	}
	
	for(int i = 0; i < MAX_CLIENTS; i++)
		close(connect_sock);
	clean_up(0,&sock);
}

void stopServer()
{
	stop = 1;
}
void handleClient(void* vars)
{
	int number;
	while(true)
	{
		printf("Please enter a number: ");
		scanf("%d", &number);
		write(vars[1], &number, 4);
		sleep(1);
		if(number == -1)
			break;
		printf("Server has written %d to socket.\n",number);
	}
}
