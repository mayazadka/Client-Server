#include "clientHandler.h"
// ----------------------------------------------------------------------------------------------------------------------
// this method is responsibal for handeling with one client. (it is run throw the new socket that is opend for each client) 
// ----------------------------------------------------------------------------------------------------------------------
void* handleClient(void* args)
{
	char* buf = malloc(100);
	info_sockh* info = args;
	printf("connected to a client throw sock : %d\n" , info->connect_sock);
	write(info->connect_sock , "hello\n" ,7);
	read(info->connect_sock , buf ,100);
	return 0;
}