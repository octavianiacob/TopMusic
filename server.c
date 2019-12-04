/* servTcpConc.c - Exemplu de server TCP concurent
    Asteapta un nume de la clienti; intoarce clientului sirul
    "Hello nume".
    */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/* portul folosit */
#define PORT 2024
#define BUF 1000

/* codul de eroare returnat de anumite apeluri */
extern int errno;

int main()
{
	struct sockaddr_in server; // structura folosita de server
	struct sockaddr_in from;
	char input[1000];		 //mesajul primit de la client
	char output[1000] = " "; //mesaj de raspuns pentru client
	int sd;					 //descriptorul de socket
	char user_input[1000];   // copie a mesajului primit de la client

	// conexiune baza de date
	sqlite3 *database;
	sqlite3_stmt *statement;
	int rc;
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database \n");
	else
		printf("Database opened successfully \n");

	/*rc = sqlite3_prepare_v2(database, "SELECT * from admin", -1, &statement, 0);
	if (rc != SQLITE_OK)
	{
		printf("Error fetching data \n");
		sqlite3_close(database);
	}

	rc = sqlite3_step(statement);
	if (rc == SQLITE_ROW)
	{
		printf("%s\n", sqlite3_column_text(statement, 0));
	}

	sqlite3_finalize(statement);
	sqlite3_close(database); */

	//sqlite3_close(database);

	/* crearea unui socket */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Eroare la socket().\n");
		return errno;
	}

	/* pregatirea structurilor de date */
	bzero(&server, sizeof(server));
	bzero(&from, sizeof(from));

	/* umplem structura folosita de server */
	/* stabilirea familiei de socket-uri */
	server.sin_family = AF_INET;
	/* acceptam orice adresa */
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	/* utilizam un port utilizator */
	server.sin_port = htons(PORT);

	/* atasam socketul */
	if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("Eroare la bind().\n");
		return errno;
	}

	/* punem serverul sa asculte daca vin clienti sa se conecteze */
	if (listen(sd, 1) == -1)
	{
		perror("Eroare la listen().\n");
		return errno;
	}

	/* servim in mod concurent clientii... */
	while (1)
	{
		int client;
		int length = sizeof(from);

		printf("Listening on port %d...\n", PORT);
		fflush(stdout);

		/* acceptam un client (stare blocanta pina la realizarea conexiunii) */
		client = accept(sd, (struct sockaddr *)&from, &length);

		/* eroare la acceptarea conexiunii de la un client */
		if (client < 0)
		{
			perror("Eroare la accept().\n");
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
			// parinte
			close(client);
			continue;
		}
		else if (pid == 0)
		{

			while (1)
			{
				char username[100], password[100];
				// copil
				close(sd);
				/* s-a realizat conexiunea, se astepta mesajul */
				bzero(input, 1000);
				printf("Welcome to TopMusic - Server\n");
				//printf("%s\n", sqlite3_libversion());
				printf("Awaiting input from client...\n");
				fflush(stdout);

				/* citirea mesajului */
				if (read(client, input, BUF) <= 0)
				{
					perror("Eroare la read() de la client.\n");
					close(client); /* inchidem conexiunea cu clientul */
					continue;	  /* continuam sa ascultam */
				}

				printf(" Input received: %s\n", input);

				// --- FUNCTIE EXIT ------------

				if (strcmp(input, "exit") == 0)
				{
					printf("Ending connection with client...");
					write(client, "Connection terminated.\n", BUF);
					close(client);
					exit(0);
				}

				// --- FUNCTIE REGISTER --------

				else if (strcmp(input, "register") == 0)
				{
					bzero(username, 100);
					bzero(password, 100);
					read(client, username, BUF);
					printf("Username-ul este %s \n", username);
					read(client, password, BUF);
					printf("Parola este %s \n", password);
					
				}

				// --- FUNCTIE LOGIN -----------

				else if (strcmp(input, "login") == 0)
				{
					bzero(username, 100);
					bzero(password, 100);
					read(client, username, BUF);
					printf("Username-ul este %s \n", username);
					read(client, password, BUF);
					printf("Parola este %s \n", password);
				}

				else
				{
					bzero(output, BUF);
					strcpy(output, "Error! Unknown command.\n");
					bzero(input, BUF);
								}

				/*pregatim mesajul de raspuns */
				//bzero(output, BUF);
				//strcat(output, "Hello ");
				//strcat(output, input);

				printf(" Sending response back to client: %s\n", output);

				/* returnam mesajul clientului */
				if (write(client, output, BUF) <= 0)
				{
					perror("Eroare la write() catre client.\n");
					continue; /* continuam sa ascultam */
				}
				else
					printf(" Response sent successfully.\n");
			} //while
		}	 // else if
	}		  //while
} //main