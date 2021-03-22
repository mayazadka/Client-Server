#include "clientHandler.h"

// ----------------------
// handling an OBD client
// ----------------------
void handleOBDclient(MYSQL * con, int client, char* car_id, char* customer_id, char* password)
{
	char* query, *hash;
	char *customer_car_id, *drive_id;
	MYSQL_RES *result;
	MYSQL_ROW row;
	Strings argumants;
	int n;
	char buf[1000];

	// query: get the salt and the hashed password
	query = appendStrings(3, "SELECT salt,hashed_password FROM car WHERE car_id = '", car_id, "';");
	result = queryAndResponse(1, con, query);
	free(query);
	if(result == NULL)
	{
		write(client, "error with query or get result", 30);
		return;
	}
	row = mysql_fetch_row(result);
	if(row == NULL)
	{
		return;
	}
	char* salt = row[0];
	char* hashed_password = row[1];
	mysql_free_result(result);

	// input validation
	if(!emailPattern(password) || !onlyLettersAndNumbers(car_id) || !onlyNumbers(customer_id))
	{
		return;
	}

	// check the password
	hash = GenerateSaltedHash(password,salt);
	if(strcmp(hash,hashed_password) != 0)
	{
		return;
	}
	free(hash);

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
	
	// query: get the customer_car_id for the current user drive
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

// ----------------------------
// handling a statistic manager
// ----------------------------
void handleStatisticInformationManager(MYSQL * con, int client, char* drive_id, char* username, char* password)
{
	char* query, *hash;
	MYSQL_RES *result;
	MYSQL_ROW row;
	Strings argumants;
	int n;
	char buf[1000];

	// query: get the salt and the hashed password
	query = appendStrings(3, "SELECT salt,hashed_password FROM statistic_information_manager WHERE username = '", username, "';");
	result = queryAndResponse(1, con, query);
	free(query);
	if(result == NULL)
	{
		write(client, "error with query or get result", 30);
		return;
	}
	row = mysql_fetch_row(result);
	if(row == NULL)
	{
		return;
	}
	char* salt = row[0];
	char* hashed_password = row[1];
	mysql_free_result(result);

	// input validation
	if(!onlyNumbers(drive_id) || !emailPattern(password))
	{
		return;
	}

	// check the password
	hash = GenerateSaltedHash(password,salt);
	if(strcmp(hash,hashed_password) != 0)
	{
		return;
	}
	free(hash);

	write(client, "ok", 2);
	
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
						// query: get statistic paramater about a drive
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

// ------------------------
// handling a worker client
// ------------------------
void handleWorker(MYSQL * con, int client, char* worker_id, char* password)
{
	char* query, *hash;
	MYSQL_RES *result;
	MYSQL_ROW row;
	Strings argumants;
	int n;
	char buf[1000];

	// query: get the salt and the hashed password
	query = appendStrings(3, "SELECT salt,hashed_password,permitions FROM worker WHERE worker_id = '", worker_id, "';");
	result = queryAndResponse(1, con, query);
	free(query);
	if(result == NULL)
	{
		write(client, "error with query or get result", 30);
		return;
	}
	row = mysql_fetch_row(result);
	if(row == NULL)
	{
		return;
	}
	char* salt = row[0];
	char* hashed_password = row[1];
	mysql_free_result(result);

	// input validation
	if(!onlyNumbers(worker_id) || !emailPattern(password))
	{
		return;
	}

	// check the password
	hash = GenerateSaltedHash(password,salt);
	if(strcmp(hash,hashed_password) != 0)
	{
		return;
	}
	free(hash);

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
			if(argumants.size != 5)
			{
				write(client, "USE: add_car [car id] [manufacturing year] [model] [password]", 61);
			}
			else
			{
				if(!onlyLettersAndNumbers(argumants.strings[1]) || !onlyNumbers(argumants.strings[2]) || !onlyLettersAndNumbers(argumants.strings[3]) || !emailPattern(argumants.strings[4]))
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
						char* salt = getSalt();
						char* hashed_password = GenerateSaltedHash(argumants.strings[4], salt);
						if(salt ==NULL || hashed_password == NULL)
						{
							write(client, "error allocation", 16);
						}
						else
						{
							// query: add car to db
							query = appendStrings(11,"INSERT INTO car (car_id, manufacturing_year, model, hashed_password, salt) VALUES( \
													'",argumants.strings[1],"',\
													",argumants.strings[2],",\
													'",argumants.strings[3],"',\
													'",hashed_password,"',\
													'",salt,"');"); 
							queryAndResponse(0, con, query);
							free(query);
							free(salt);
							free(hashed_password);
							write(client, "ok", 2);
						}
						
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

// ----------------------------------------------------------------------------------------------------------------------
// main function for handling any client
// ----------------------------------------------------------------------------------------------------------------------
void* handleClient(void* args)
{
	char buf[1000];
	Specific_data* specific_data = args;
	int n;
	Strings argumants;
	MYSQL *con;

	con = mysql_init(NULL); // initialize connection to mysql
	if (con == NULL)
	{
		write(specific_data->connect_sock, "error initialize connection to db", 33);
		close(specific_data->connect_sock);
		return NULL;
	}
	if(mysql_real_connect(con, MYSQLIP, MYSQLUSER, MYSQLPASSWORD, MYSQLDB, 0, NULL, 0) == NULL) // connect to db
	{
		write(specific_data->connect_sock, "error connect to db", 19);
		close(specific_data->connect_sock);
		mysql_close(con);
		return NULL;
	}

	n = read(specific_data->connect_sock, buf, 1000); //read first command from client
	buf[n] = '\0';
	argumants = split(buf);
	if(argumants.error != 1 && argumants.size != 0)
	{
		if(strcmp(argumants.strings[0], "connect") == 0 && argumants.size == 4) // OBDclient
		{
			handleOBDclient(con, specific_data->connect_sock, argumants.strings[1], argumants.strings[2], argumants.strings[3]);
		}
		else if (strcmp(argumants.strings[0], "statistic") == 0 && argumants.size == 4) // Statistic Manager
		{
			handleStatisticInformationManager(con, specific_data->connect_sock, argumants.strings[1], argumants.strings[2], argumants.strings[3]);
		}
		else if(strcmp(argumants.strings[0], "worker") == 0 && argumants.size == 3) // worker
		{
			handleWorker(con, specific_data->connect_sock, argumants.strings[1], argumants.strings[2]);
		}
	}
	
	// end connection with client
	write(specific_data->connect_sock, "bye", 3);
	close(specific_data->connect_sock);

	mysql_close(con);
	freeStrings(argumants);

	addLast(specific_data->available, specific_data->place);
	return NULL;
}

// ----------------------------------------------------------------------------------------------------------------------
// send the query to the db and returns the response
// ----------------------------------------------------------------------------------------------------------------------
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