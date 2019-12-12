#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

#define PORT 2024
#define BUF 1000

extern int errno;

char username[100], password[100];
int loginFlag, adminFlag;
sqlite3 *database;
sqlite3_stmt *statement;
char *query = NULL;
char *err_msg = 0;
char str[1000];
int rc;

void insertUser(char username[])
{
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "insert into user (user_name) values (\"%s\");", username);
	printf("The following SQL Query will run: '%s'\n", query);
	sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE)
		printf("ERROR inserting data: %s\n", sqlite3_errmsg(database));
	else
		printf("Username inserted successfully.\n");
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
}

void insertPassword(char username[], char password[])
{
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "update user set user_password = \"%s\" where user_name = \"%s\";", password, username);
	printf("The following SQL Query will run: '%s'\n", query);
	sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE)
		printf("ERROR inserting data: %s\n", sqlite3_errmsg(database));
	else
		printf("Password inserted successfully.\n");
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
}

int isValidUser(char username[])
{
	char isValid[1000];
	bzero(isValid, 1000);
	isValid[0] = '1';
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM user WHERE user_name = \"%s\";", username);
	printf("The following SQL Query will run: '%s'\n", query);
	rc = sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc == SQLITE_ROW)
		sprintf(isValid, sqlite3_column_text(statement, 0));
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	if (strcmp(isValid, "1") == 0)
		return 1;
	return 0;
}

int isAdmin(char username[], char password[])
{
	char isAdmin[BUF];
	bzero(isAdmin, BUF);
	isAdmin[0] = '0';
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM admin WHERE admin_name = \"%s\" AND  admin_password = \"%s\";", username, password);
	printf("The following SQL Query will run: '%s'\n", query);
	rc = sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc == SQLITE_ROW)
		sprintf(isAdmin, sqlite3_column_text(statement, 0));
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);

	if (strcmp(isAdmin, "0") == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int isLoggedIn(char username[], char password[])
{
	char isLoggedIn[BUF];
	bzero(isLoggedIn, BUF);
	isLoggedIn[0] = '0';
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM user WHERE user_name = \"%s\" AND  user_password = \"%s\";", username, password);
	printf("The following SQL Query will run: '%s'\n", query);
	rc = sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc == SQLITE_ROW)
		sprintf(isLoggedIn, sqlite3_column_text(statement, 0));
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);

	if (strcmp(isLoggedIn, "0") == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void insertSong(char song_name[], char song_artist[], char song_link[], char genre1, char genre2, char genre3)
{
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database.\n");
	else
		printf("Database opened successfully.\n");
	//asprintf(&query, "insert into songs (song_name, song_artist, link, genre1, genre2, genre3) values (\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\");", song_name, song_artist, song_link, genre1, genre2, genre3);
	asprintf(&query, "insert into songs (song_name, song_artist, link, genre1) values (\"%s\", \"%s\", \"%s\", \"%s\");", song_name, song_artist, song_link);
	printf("The following SQL Query will run: '%s'\n", query);
	sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE)
		printf("ERROR inserting data: %s\n", sqlite3_errmsg(database));
	else
		printf("Song inserted successfully.\n");
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
}

void insertGenre(char genre_name[], char genre_description[])
{
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database.\n");
	else
		printf("Database opened successfully.\n");
	asprintf(&query, "insert into genres (genre_name, genre_description) values (\"%s\", \"%s\");", genre_name, genre_description);
	printf("The following SQL Query will run: '%s'\n", query);
	sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE)
		printf("ERROR inserting data: %s\n", sqlite3_errmsg(database));
	else
		printf("Genre inserted successfully.\n");
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
}

void showGenres(char* out)
{
	int genre_count = 0;
	char genresDisplay[1000];
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM genres;");
	printf("The following SQL Query will run: '%s'\n", query);
	if (sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL) != SQLITE_OK)
	{
		sqlite3_close(database);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(database));
	}
	strcat(genresDisplay, "ID     Genre     Description\n----------------------------\n \n");
	while (sqlite3_step(statement) == SQLITE_ROW)
	{
			   strcat(genresDisplay, "  ");
			   strcat(genresDisplay, sqlite3_column_text(statement, 0));
			   strcat(genresDisplay, "     ");
			   strcat(genresDisplay, sqlite3_column_text(statement, 1));
			   strcat(genresDisplay, "     ");
			   strcat(genresDisplay, sqlite3_column_text(statement, 2));
			   strcat(genresDisplay, "\n");
		genre_count++;
	}
	printf("Number of genres: %d\n", genre_count);
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	//printf("Variabila genresDisplay are:\n %s\n", genresDisplay);
	strcpy(out,genresDisplay);
}

int main()
{
	struct sockaddr_in server;
	struct sockaddr_in from;
	char input[1000];		 //mesajul primit de la client
	char output[1000] = " "; //mesaj de raspuns pentru client
	int sd;					 
	char user_input[1000];   // copie a mesajului primit de la client

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Error on socket().\n");
		return errno;
	}

	bzero(&server, sizeof(server));
	bzero(&from, sizeof(from));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);

	if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("Error on bind().\n");
		return errno;
	}

	if (listen(sd, 1) == -1)
	{
		perror("Error on listen().\n");
		return errno;
	}

	while (1) // Asigura conexiunea la toti clientii
	{
		int client;
		int length = sizeof(from);
		loginFlag = 0;
		adminFlag = 0;

		printf("Listening on port %d...\n", PORT);
		fflush(stdout);

		client = accept(sd, (struct sockaddr *)&from, &length);

		if (client < 0)
		{
			perror("Error on accept().\n");
			continue;
		}

		int pid;
		if ((pid = fork()) == -1)
		{
			close(client);
			continue;
		}
		else if (pid > 0)
		{
			close(client);
			continue;
		}
		else if (pid == 0)
		{

			while (1)
			{
				close(sd);
				bzero(input, 1000);
				printf("Welcome to TopMusic - Server\n");
				printf("Awaiting input from client...\n");
				printf("LOGIN FLAG = %d , ADMIN FLAG = %d \n", loginFlag, adminFlag);
				fflush(stdout);

				if (read(client, input, BUF) <= 0)
				{
					perror("Error on read() from client.\n");
					close(client);
					continue;
				}

				printf(" Input received: %s\n", input);

				// --- FUNCTIE EXIT ------------

				if (strcmp(input, "exit") == 0)
				{
					printf("Ending connection with client...\n");
					//write(client, "Connection terminated.\n", BUF);
					close(client);
					exit(0);
				}

				// --- FUNCTIE REGISTER --------

				else if (strcmp(input, "register") == 0)
				{
					bzero(username, 100);
					bzero(password, 100);
					read(client, username, BUF); // citeste username de la client (1)
					printf("Username is: %s \n", username);

					if (isValidUser(username) == 1)
					{
						bzero(output, BUF);
						strcat(output, "valid");
						write(client, output, BUF); //scrie valid la client (2)
						bzero(output, BUF);
						printf("USERNAME VALID \n");
						insertUser(username);
						read(client, password, BUF); //citeste parola de la client (3)
						printf("Password is: %s \n", password);
						insertPassword(username, password);
					}
					else
					{
						bzero(output, BUF);
						strcat(output, "invalid");
						write(client, output, BUF);
						printf("USERNAME INVALID \n");
					}
				}

				// --- FUNCTIE LOGIN -----------

				else if (strcmp(input, "login") == 0)
				{
					bzero(username, 100);
					bzero(password, 100);
					read(client, username, BUF); // citeste username de la client (1)
					printf("Username is: %s \n", username);
					read(client, password, BUF); // citeste parola de la client (2)
					printf("Password is: %s \n", password);
					if (isAdmin(username, password) == 1)
					{
						printf("IS ADMIN\n");
						adminFlag = 1;
						loginFlag = 1;
						bzero(output, BUF);
						strcat(output, "admin");
						write(client, output, BUF); // scrie logat ca admin la client(3)
					}

					else if (isLoggedIn(username, password) == 1)
					{
						printf("IS REGULAR USER\n");
						adminFlag = 0;
						loginFlag = 1;
						bzero(output, BUF);
						strcat(output, "user");
						write(client, output, BUF); // scrie logat ca user la client(3)
					}
					else
					{
						printf("WRONG CREDENTIALS\n");
						adminFlag = 0;
						loginFlag = 0;
						bzero(output, BUF);
						strcat(output, "error");
						write(client, output, BUF); // scrie eroare la login la client(3)
					}
				}

				// --- FUNCTIE ADD SONG -----

				else if (strcmp(input, "addSong") == 0)
				{
					char song_name[BUF], song_artist[BUF], song_link[BUF], genre1[BUF], genre2[BUF], genre3[BUF];

					read(client, song_name, BUF); // citeste song_name de la client (1);
					printf("Song name is: %s \n", song_name);
					read(client, song_artist, BUF); // citeste song_artist de la client (2);
					printf("Song artist is: %s \n", song_artist);
					read(client, song_link, BUF); // citeste link de la client (3);
					printf("link is: %s \n", song_link);
					read(client, genre1, BUF); // citeste genre1 de la client (4);
					printf("Song genre1 is: %s \n", genre1);
					read(client, genre2, BUF); // citeste genre2 de la client (5);
					printf("Song genre2 is: %s \n", genre2);
					read(client, genre3, BUF); // citeste genre3 de la client (6);
					printf("Song genre3 is: %s \n", genre3);
					insertSong(song_name, song_artist, song_link, genre1, genre2, genre3);
					printf("Song inserted successfully.\n");
				}

				// --- FUNCTIE ADD GENRES -----

				else if (strcmp(input, "addGenre") == 0)
				{
					char genre_name[BUF], genre_description[BUF];
					read(client, genre_name, BUF); // citeste genre name de la client (1);
					printf("Song name is: %s \n", genre_name);
					read(client, genre_description, BUF); // citeste genre description de la client (2);
					printf("Song artist is: %s \n", genre_description);
					if (strcmp(genre_description, "") == 0)
						strcat(genre_description, "No description");
					insertGenre(genre_name, genre_description);
					printf("Genre inserted successfully.\n");
				}

				// --- FUNCTIE SHOW GENRES

				else if (strcmp(input, "showGenres") == 0)
				{
					printf("A intrat pe functie\n");
					char gen[1000];
					showGenres(gen);
					printf("GEN: %s\n", gen);
					write(client, gen, 1000); // trimite la client genres
				}

				// --- CAZ EROARE -----------

				else
				{
					printf("The following command failed: %s\n", output);
					bzero(output, BUF);
					strcpy(output, "Error! Unknown command.\n");
					bzero(input, BUF);
				}
			} //while
		}	 // else if
	}		  //while
} //main