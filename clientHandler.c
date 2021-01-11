#include "clientHandler.h"

void* handleClient(void* args)
{
    int number;
	info_sockh* info = args;
	while(1)
	{
		printf("Please enter a number: ");
		scanf("%d", &number);
		write(info->connect_sock, &number, 4);
		sleep(1);
		if(number == -1)
			break;
		printf("Server has written %d to socket.\n",number);
	}
	return 0;
}