#include "clientHandler.h"

// ----------------------------------------------------------------------------------------------------------------------
// this method is responsibal for handeling with one client. (it is run throw the new socket that is opend for each client) 
// ----------------------------------------------------------------------------------------------------------------------
void* handleClient(void* args)
{
	char buf[1000];
	info_sockh* info = args;
	int n;
	Strings strings;
	char *customer_id, *car_id, *customer_car_id, *drive_id;
	MYSQL *con = mysql_init(NULL);
	MYSQL_RES *result;
	MYSQL_ROW row;
	char* query;

	if (con == NULL)
	{
		// error to log: mysql_error(con);
		write(info->connect_sock, "error", 5);
		close(info->connect_sock);
		return NULL;
	}
	if(mysql_real_connect(con, "127.0.0.1", "root", "529123",  "ottomate", 0, NULL, 0) == NULL)
	{
		// error to log: mysql_error(con);
		write(info->connect_sock, "error", 5);
		close(info->connect_sock);
		mysql_close(con);
		return NULL;
	}
	strings.size = 0;
	do
	{
		freeStrings(strings);
		n = read(info->connect_sock, buf, 1000);
		buf[n] = '\0';
		strings = split(buf);
		if(strings.error == 1)
		{
			write(info->connect_sock, "error", 5);
			close(info->connect_sock);
			mysql_close(con);
			return NULL;
		}
		if(strings.size == 0)
		{
			write(info->connect_sock, "command error", 13);
		}
		else if(strcmp(strings.strings[0], "quit") == 0)
		{
			write(info->connect_sock, "error", 5);
			close(info->connect_sock);
			mysql_close(con);
			freeStrings(strings);
			return NULL;
		}
		else if(strcmp(strings.strings[0], "connect") != 0)
		{
			write(info->connect_sock, "must connect", 12);
		}
		else if(strings.size != 3)
		{
			write(info->connect_sock, "USE: connect [car id] [customer id]", 35);
		}
		else{
			car_id = (char*)malloc(sizeof(char)*strlen(strings.strings[1]));
			if(car_id == NULL)
			{
				write(info->connect_sock, "error", 5);
				close(info->connect_sock);
				mysql_close(con);
				freeStrings(strings);
				return NULL;
			}
			strcpy(car_id, strings.strings[1]);
			customer_id = (char*)malloc(sizeof(char)*strlen(strings.strings[2]));
			if(car_id == NULL)
			{
				write(info->connect_sock, "error", 5);
				close(info->connect_sock);
				mysql_close(con);
				free(car_id);
				freeStrings(strings);
				return NULL;
			}
			strcpy(customer_id, strings.strings[2]);
			query = appendStrings(5, "SELECT COUNT(car_id) FROM customer_car WHERE car_id = '", car_id, "' AND customer_id = '", customer_id, "';");
			result = queryAndResponse(1, con, query);
			free(query);
			if(result == NULL)
			{
				write(info->connect_sock, "error", 5);
				close(info->connect_sock);
				mysql_close(con);
				freeStrings(strings);
				free(car_id);
				free(customer_id);
				return NULL;
			}
			row = mysql_fetch_row(result);
			if(strcmp(row[0], "0") == 0){
				write(info->connect_sock, "Requested an unexisting client", 30);
				free(car_id);
				free(customer_id);
			}
			else {
				write(info->connect_sock, "Valid user", 10);
			}
			mysql_free_result(result);
		}
	} while (strings.size == 0 || strcmp(strings.strings[0], "connect") != 0 || strings.size != 3 || strcmp(row[0], "0") == 0);
	freeStrings(strings);
	query = appendStrings(5, "SELECT customer_car_id FROM customer_car WHERE car_id = '", car_id, "' AND customer_id = '", customer_id, "';");
	result = queryAndResponse(1, con, query);
	free(query);
	if(result == NULL)
	{
		write(info->connect_sock, "error", 5);
		close(info->connect_sock);
		mysql_close(con);
		free(car_id);
		free(customer_id);
		return NULL;
	}
	row = mysql_fetch_row(result);
	customer_car_id = (char*)malloc(sizeof(char) * strlen(row[0]));
	if(customer_car_id == NULL)
	{
		write(info->connect_sock, "error", 5);
		close(info->connect_sock);
		mysql_close(con);
		free(car_id);
		free(customer_id);
		mysql_free_result(result);
		return NULL;
	}
	strcpy(customer_car_id ,row[0]);
	query = appendStrings(3, "INSERT INTO drive(start_time, end_time, customer_car_id) VALUES(NOW(), NULL, ", customer_car_id, ");");
	queryAndResponse(0, con, query);
	free(query);
	puts("5");

	query = appendStrings(3, "SELECT drive_id FROM drive WHERE customer_car_id = ", customer_car_id, " ORDER BY start_time DESC LIMIT 1;");
	puts("6");
	result = queryAndResponse(1, con, query);
	free(query);
	puts("9");

	if(result == NULL)
	{
		write(info->connect_sock, "error", 5);
		close(info->connect_sock);
		mysql_close(con);
		free(car_id);
		free(customer_id);
		return NULL;
	}
	row = mysql_fetch_row(result);
	puts("7");

	drive_id = (char*)malloc(sizeof(char) * strlen(row[0]));
	if(drive_id == NULL)
	{
		write(info->connect_sock, "error", 5);
		close(info->connect_sock);
		mysql_close(con);
		free(car_id);
		free(customer_id);
		free(customer_car_id);
		mysql_free_result(result);
		return NULL;
	}
	strcpy(drive_id, row[0]);
	mysql_free_result(result);
	puts("8");

	while(1)
	{
		n = read(info->connect_sock, buf, 1000);
		buf[n] = '\0';
		strings = split(buf);
		if(strings.size == 0)
		{
			write(info->connect_sock, "command error", 13);
		}
		else if(strcmp(strings.strings[0], "quit") == 0)
		{
  			break;
		}
		else if(strcmp(strings.strings[0], "set") != 0)
		{
			write(info->connect_sock, "only set permission", 19);
		}
		else if(strings.size != 7)
		{
			write(info->connect_sock, "must send 6 arguments", 21);
		}
		else{
			query = appendStrings(15, "INSERT INTO drive_characteristics(drive_id, engine_speed, speed, throtle, engine_load, fuel, air_temperature) VALUES(", drive_id, ",", strings.strings[1], ",", strings.strings[2], ",", strings.strings[3], ",", strings.strings[4], ",", strings.strings[5], ",", strings.strings[6], ");");
			puts(query);
			queryAndResponse(0, con, query);
			free(query);
			write(info->connect_sock, "OK", 2);
		}
		freeStrings(strings);
	}
	//update end_time

	close(info->connect_sock);
	mysql_close(con);
	free(car_id);
	free(customer_id);
	free(customer_car_id);
	free(drive_id);
  	mysql_free_result(result);
	freeStrings(strings);
	return NULL;
}

MYSQL_RES * queryAndResponse(int isResult, MYSQL *con, char* query)
{
	MYSQL_RES *result = NULL;
	if(mysql_query(con, query))
	{
		puts(mysql_error(con));
		mysql_close(con);
		return NULL;
	}
	if(isResult)
	{
		result =  mysql_store_result(con);
		if(result == NULL)
		{
			puts(mysql_error(con));
			mysql_close(con);
			return NULL;
		}
	}
	return result;
}