#include "clientHandler.h"

// ----------------------------------------------------------------------------------------------------------------------
// this method is responsibal for handeling with one client. (it is run throw the new socket that is opend for each client) 
// ----------------------------------------------------------------------------------------------------------------------

void handleOBDclient(MYSQL * con, int client, char* car_id, char* customer_id)
{
	char* query;
	char *customer_car_id, *drive_id;
	MYSQL_RES *result;
	MYSQL_ROW row;
	Strings argumants;
	int n;
	char buf[1000];

	//needs validation to car_id, customer_id

	// query: check if the combination of car_id and customer_id is existing
	query = appendStrings(5, "SELECT COUNT(car_id) FROM customer_car WHERE car_id = '", car_id, "' AND customer_id = '", customer_id, "';");
	result = queryAndResponse(1, con, query);
	free(query);
	if(result == NULL)
	{
		write(client, "error with query or get result", 30);
		return;
	}
	row = mysql_fetch_row(result);
	if(strcmp(row[0], "0") == 0)
	{
		mysql_free_result(result);
		return;
	}
	mysql_free_result(result);

	write(client, "ok", 2);
	
	// query: get the customer_car_if for the current user drive
	query = appendStrings(5, "SELECT customer_car_id FROM customer_car WHERE car_id = '", car_id, "' AND customer_id = '", customer_id, "';");
	result = queryAndResponse(1, con, query);
	free(query);
	if(result == NULL)
	{
		write(client, "error with query or get result", 30);
		return;
	}
	row = mysql_fetch_row(result);
	customer_car_id = (char*)malloc(sizeof(char) * (strlen(row[0]) + 1)); // add 1 for '\0'
	if(customer_car_id == NULL)
	{
		write(client, "error with allocation", 21); 
		mysql_free_result(result);
		return;
	}
	strcpy(customer_car_id ,row[0]);
	mysql_free_result(result);

	// query: create new drive
	query = appendStrings(3, "INSERT INTO drive(start_time, end_time, customer_car_id) VALUES(NOW(), NULL, ", customer_car_id, ");");
	queryAndResponse(0, con, query);
	free(query);

	// query: get the current drive_id
	query = appendStrings(3, "SELECT drive_id FROM drive WHERE customer_car_id = ", customer_car_id, " ORDER BY start_time DESC LIMIT 1;");
	result = queryAndResponse(1, con, query);
	free(query);
	if(result == NULL)
	{
		write(client, "error with query or get result", 30);
		free(customer_car_id);
		return;
	}
	row = mysql_fetch_row(result);
	drive_id = (char*)malloc(sizeof(char) * (strlen(row[0]) +1)); // add 1 for '\0'
	if(drive_id == NULL)
	{
		write(client, "error with allocation", 21); 
		free(customer_car_id);
		mysql_free_result(result);
		return;
	}
	strcpy(drive_id, row[0]);
	mysql_free_result(result);

	while(1)
	{
		n = read(client, buf, 1000);
		buf[n] = '\0';
		argumants = split(buf);
		if(argumants.size == 0)
		{
			write(client, "must send set or quit command", 29);
		}
		else if(strcmp(argumants.strings[0], "quit") == 0)
		{
			freeStrings(argumants);
  			break;
		}
		else if(strcmp(argumants.strings[0], "set") == 0)
		{
			if(argumants.size != 7)
			{
				write(client, "USE: set [engine speed] [speed] [throtle] [engine load] [fuel] [air temperature]", 80);
			}
			else
			{
				//needs validation to charactristics

				// query: insert the drive characteristics
				query = appendStrings(15,
				"INSERT INTO \
				drive_characteristics(drive_id, engine_speed, speed, throtle, engine_load, fuel, air_temperature) \
				VALUES(", drive_id, ",", argumants.strings[1], ",", argumants.strings[2], ",", argumants.strings[3],
				",", argumants.strings[4], ",", argumants.strings[5], ",", argumants.strings[6], ");");
				queryAndResponse(0, con, query);
				free(query);
				write(client, "ok", 2);
			}
		}
		else
		{
			write(client, "must send set or quit command", 29);
		}
		freeStrings(argumants);
	}
	//update end_time

	free(customer_car_id);
	free(drive_id);
}

void handleAlgorithm(MYSQL * con, int client, char* password)
{
	write(client, "cant handle algorithm", 21);
	return;
}

void* handleClient(void* args)
{
	char buf[1000];
	info_sockh* info = args;
	int n;
	Strings argumants;
	MYSQL *con;

	con = mysql_init(NULL);
	if (con == NULL)
	{
		write(info->connect_sock, "error initialize connection to db", 33);
		close(info->connect_sock);
		return NULL;
	}
	if(mysql_real_connect(con, "127.0.0.1", "root", "OMEome0707",  "ottomate", 0, NULL, 0) == NULL)
	{
		write(info->connect_sock, "error connect to db", 19);
		close(info->connect_sock);
		mysql_close(con);
		return NULL;
	}

	n = read(info->connect_sock, buf, 1000);
	buf[n] = '\0';
	argumants = split(buf);
	if(argumants.error != 1 && argumants.size != 0)
	{
		if(strcmp(argumants.strings[0], "connect") == 0 && argumants.size == 3)
		{
			handleOBDclient(con, info->connect_sock, argumants.strings[1], argumants.strings[2]);
		}
		else if (strcmp(argumants.strings[0], "algo") == 0 && argumants.size == 2)
		{
			handleAlgorithm(con, info->connect_sock, argumants.strings[1]);
		}
	}
	
	write(info->connect_sock, "bye", 3);

	close(info->connect_sock);
	mysql_close(con);
	freeStrings(argumants);
	return NULL;
}



MYSQL_RES * queryAndResponse(int isResult, MYSQL *con, char* query)
{
	MYSQL_RES *result = NULL;
	if(mysql_query(con, query))
	{
		puts(mysql_error(con));
		return NULL;
	}
	if(isResult)
	{
		result =  mysql_store_result(con);
		if(result == NULL)
		{
			puts(mysql_error(con));
			return NULL;
		}
	}
	return result;
}