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

	if(!onlyLettersAndNumbers(car_id) || !onlyNumbers(customer_id))
	{
		return;
	}

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
	drive_id = (char*)malloc(sizeof(char) * (strlen(row[0]) +1));
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
	
	// query: update end time
	query = appendStrings(3, "UPDATE drive SET end_time = NOW() WHERE drive_id = '", drive_id, "';");
	queryAndResponse(0, con, query);
	free(query);

	free(customer_car_id);
	free(drive_id);
}

void handleAlgorithm(MYSQL * con, int client, char* drive_id, char* password)
{
	char* query;
	MYSQL_RES *result;
	MYSQL_ROW row;
	Strings argumants;
	int n;
	char buf[1000];

	// check the password
	if(strcmp(password, "qwerty") != 0)
	{
		return;
	}

	write(client, "ok", 2);
	
	
	if(!onlyNumbers(drive_id))
	{
		return;
	}

	// query: check if the drive_id is existing
	query = appendStrings(3, "SELECT COUNT(drive_id) FROM drive WHERE drive_id = '", drive_id, "';");
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

	while(1)
	{
		n = read(client, buf, 1000);
		buf[n] = '\0';
		argumants = split(buf);
		if(argumants.size == 0)
		{
			write(client, "must send get command", 21);
		}
		else if(strcmp(argumants.strings[0], "quit") == 0)
		{
			freeStrings(argumants);
  			break;
		}
		else if(strcmp(argumants.strings[0], "get") == 0)
		{
			if(argumants.size < 2)
			{
				write(client, "USE: get [argument] [...]", 31);
			}
			else
			{
				if(strcmp(argumants.strings[1], "speed") == 0 || strcmp(argumants.strings[1], "engine_speed") == 0 || strcmp(argumants.strings[1], "throtle") == 0 || strcmp(argumants.strings[1], "engine_load") == 0 || strcmp(argumants.strings[1], "fuel") == 0 || strcmp(argumants.strings[1], "air_temperature") == 0)
				{
					if(argumants.size != 3 || (strcmp(argumants.strings[2], "MIN") != 0 && strcmp(argumants.strings[2], "MAX") != 0 && strcmp(argumants.strings[2], "AVG") != 0))
					{
						write(client,  "USE: get [paramater name] [MIN/MAX/AVG]", 39);
					}
					else
					{
						query = appendStrings(7,
						"SELECT ",argumants.strings[2],"(",argumants.strings[1],") \
						FROM drive_characteristics \
						WHERE drive_id = '", drive_id, "';");
						result = queryAndResponse(1, con, query);
						free(query);
						if(result == NULL)
						{
							write(client, "error with query or get result", 30);	
						}
						else
						{
							row = mysql_fetch_row(result);
							write(client, row[0], strlen(row[0]));
							mysql_free_result(result);
						}
					}
				}
				else
				{
					write(client,  "no such argument for get", 24);
				}
			}
		}
		else
		{
			write(client, "must send get command", 21);
		}
		freeStrings(argumants);
	}
}

void handleManager(MYSQL * con, int client, char* username, char* passwrod)
{
	char* query;
	MYSQL_RES *result;
	MYSQL_ROW row;
	Strings argumants;
	int n;
	char buf[1000];
	
	if(!onlyLettersAndNumbers(username))
	{
		return;
	}

	// query: check if the manager and password are correct
	query = appendStrings(5, "SELECT COUNT(username) FROM manager WHERE username = '", username, "' AND password = '",passwrod,"';");
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

	while(1)
	{
		n = read(client, buf, 1000);
		buf[n] = '\0';
		argumants = split(buf);
		if(argumants.size == 0)
		{
			write(client, "must send add_customer/add_car/combine command", 46);
		}
		else if(strcmp(argumants.strings[0], "quit") == 0)
		{
			freeStrings(argumants);
  			break;
		}
		else if(strcmp(argumants.strings[0], "add_customer") == 0)
		{
			if(argumants.size != 11)
			{
				write(client, "USE: add_customer [customer_id] [first name] [last name] [age] [country] [city] [street] [home number] [phone number] [e-mail]", 126);
			}
			else
			{
				if(!onlyNumbers(argumants.strings[1]) || !onlyLetters(argumants.strings[2]) || !onlyLetters(argumants.strings[3]) || !onlyNumbers(argumants.strings[4]) ||
				!onlyLetters(argumants.strings[5]) || !onlyLetters(argumants.strings[6]) || !onlyLetters(argumants.strings[7]) || !onlyNumbers(argumants.strings[8]) ||
				!onlyNumbers(argumants.strings[9]) || !emailPattern(argumants.strings[10]))
				{
					write(client, "invalid input", 13);
				}
				else
				{
					// query: check if the customer_id is existing
					query = appendStrings(3, "SELECT COUNT(customer_id) FROM customer WHERE customer_id = '", argumants.strings[1], "';");
					result = queryAndResponse(1, con, query);
					free(query);
					if(result == NULL)
					{
						write(client, "error with query or get result", 30);
						return;
					}
					row = mysql_fetch_row(result);
					if(strcmp(row[0], "0") != 0)
					{
						write(client, "customer id allready exist", 26);
						mysql_free_result(result);
					}
					else
					{
						mysql_free_result(result);

						// query: add customer to db
						query = appendStrings(21,"INSERT INTO customer (customer_id, first_name, last_name, age, country, city, street, home_number, phone_number, email) VALUES( \
												'",argumants.strings[1],"',\
												'",argumants.strings[2],"',\
												'",argumants.strings[3],"',\
												",argumants.strings[4],",\
												'",argumants.strings[5],"',\
												'",argumants.strings[6],"',\
												'",argumants.strings[7],"',\
												",argumants.strings[8],",\
												'",argumants.strings[9],"',\
												'",argumants.strings[10],"');"); 
						queryAndResponse(0, con, query);
						free(query);

						write(client, "ok", 2);
					}
				}
			}
		}
		else if(strcmp(argumants.strings[0], "add_car") == 0)
		{
			if(argumants.size != 4)
			{
				write(client, "USE: add_car [car id] [manufacturing year] [model]", 50);
			}
			else
			{
				if(!onlyLettersAndNumbers(argumants.strings[1]) || !onlyNumbers(argumants.strings[2]) || !onlyLettersAndNumbers(argumants.strings[3]))
				{
					write(client, "invalid input", 13);
				}
				else
				{
					// query: check if the car_id is existing
					query = appendStrings(3, "SELECT COUNT(car_id) FROM car WHERE car_id = '", argumants.strings[1], "';");
					result = queryAndResponse(1, con, query);
					free(query);
					if(result == NULL)
					{
						write(client, "error with query or get result", 30);
						return;
					}
					row = mysql_fetch_row(result);
					if(strcmp(row[0], "0") != 0)
					{
						write(client, "car id allready exist", 21);
						mysql_free_result(result);
					}
					else
					{
						mysql_free_result(result);

						// input validation

						// query: add car to db
						query = appendStrings(7,"INSERT INTO car (car_id, manufacturing_year, model) VALUES( \
												'",argumants.strings[1],"',\
												",argumants.strings[2],",\
												'",argumants.strings[3],"');"); 
						queryAndResponse(0, con, query);
						free(query);

						write(client, "ok", 2);
					}
				}
			}
		}
		else if(strcmp(argumants.strings[0], "combine") == 0)
		{
			if(argumants.size != 3)
			{
				write(client, "USE: combine [car id] [customer id]", 35);
			}
			else
			{
				if(!onlyLettersAndNumbers(argumants.strings[1]) || !onlyNumbers(argumants.strings[2]))	
				{
					write(client, "invalid input", 13);
				}
				else
				{
					// query: check if the car_id is existing
					query = appendStrings(3, "SELECT COUNT(car_id) FROM car WHERE car_id = '", argumants.strings[1], "';");
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
						write(client, "car id is not exist", 19);
						mysql_free_result(result);
					}
					else
					{
						mysql_free_result(result);

						// query: check if the customer_id is existing
						query = appendStrings(3, "SELECT COUNT(customer_id) FROM customer WHERE customer_id = '", argumants.strings[2], "';");
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
							write(client, "customer id is not exist", 24);
							mysql_free_result(result);
						}
						else
						{
							mysql_free_result(result);

							// query: check if the customer_id and car_id is existing
							query = appendStrings(5, "SELECT COUNT(customer_car_id) FROM customer_car WHERE car_id ='",argumants.strings[1],"' AND customer_id = '", argumants.strings[2], "';");
							result = queryAndResponse(1, con, query);
							free(query);
							if(result == NULL)
							{
								write(client, "error with query or get result", 30);
								return;
							}
							row = mysql_fetch_row(result);
							if(strcmp(row[0], "0") != 0)
							{
								write(client, "customer and car allready combines", 34);
								mysql_free_result(result);
							}
							else
							{
								mysql_free_result(result);

							// query: add customer_car to db
								query = appendStrings(5,"INSERT INTO customer_car (car_id, customer_id) VALUES( \
														'",argumants.strings[1],"',\
														'",argumants.strings[2],"');"); 
								queryAndResponse(0, con, query);
								free(query);

								write(client, "ok", 2);
							}
						}
					}
				}
			}
		}
		else
		{
			write(client, "must send add_customer/add_car/combine command", 46);
		}
		freeStrings(argumants);
	}
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
		else if (strcmp(argumants.strings[0], "algo") == 0 && argumants.size == 3)
		{
			handleAlgorithm(con, info->connect_sock, argumants.strings[1], argumants.strings[2]);
		}
		else if(strcmp(argumants.strings[0], "manager") == 0 && argumants.size == 3)
		{
			handleManager(con, info->connect_sock, argumants.strings[1], argumants.strings[2]);
		}
	}
	
	write(info->connect_sock, "bye", 3);
	close(info->connect_sock);

	mysql_close(con);
	freeStrings(argumants);

	addLast(info->available, info->place);
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
			return NULL;
		}
	}
	return result;
}