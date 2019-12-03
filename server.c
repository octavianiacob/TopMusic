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

/* codul de eroare returnat de anumite apeluri */
extern int errno;
 
 void registration(char input[1000]) 
 {

 }

int quit = 0;
int contor = 0;

int main ()
{
    struct sockaddr_in server;	// structura folosita de server
    struct sockaddr_in from;
    char msg[1000];		//mesajul primit de la client
    char msgrasp[1000]=" ";        //mesaj de raspuns pentru client
    int sd;			//descriptorul de socket
	char user_input[1000]; // copie a mesajului primit de la client

	// deschiderea bd-ului
	sqlite3 *database;
	int rc;
	rc = sqlite3_open("musicdb", &database);
	if(rc)
		printf("Error opening database \n");
	else
		printf("Database opened successfully \n");
	sqlite3_close(database);

    /* crearea unui socket */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
    	perror ("Eroare la socket().\n");
    	return errno;
    }

    /* pregatirea structurilor de date */
    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    /* utilizam un port utilizator */
    server.sin_port = htons (PORT);

    /* atasam socketul */
    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
    	perror ("Eroare la bind().\n");
    	return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen (sd, 1) == -1)
    {
    	perror ("Eroare la listen().\n");
    	return errno;
    }

    /* servim in mod concurent clientii... */
    while (1 && quit==0)
    {
		if(quit == 1)
		{
			break;
		}
    	int client;
    	int length = sizeof (from);

    	printf (" Listening on port %d...\n",PORT);
    	fflush (stdout);

    	/* acceptam un client (stare blocanta pina la realizarea conexiunii) */
    	client = accept (sd, (struct sockaddr *) &from, &length);

    	/* eroare la acceptarea conexiunii de la un client */
    	if (client < 0)
    	{
    		perror ("Eroare la accept().\n");
    		continue;
    	}

    	int pid;
    	if ((pid = fork()) == -1) {
    		close(client);
    		continue;
    	} else if (pid > 0) {
    		// parinte
    		close(client);
    		continue;
    	} else if (pid == 0) {

		// --- MESAJ WELCOME PENTRU CLIENT -------------

			/*pregatim mesajul de raspuns */
    		bzero(msgrasp,1000);
    		strcat(msgrasp,"Welcome to TopMusic!\n Please enter one of the following commands:\n 1) register <username> <password>\n 2) login <username> <password>\n 3) exit\n");
    		printf(" Sending welcome message to client...");

    		/* returnam mesajul clientului */
    		if (write (client, msgrasp, 1000) <= 0)
    		{
    			perror ("Eroare la write() catre client.\n");
    			continue;		/* continuam sa ascultam */
    		}
    		else
    			printf (" Response sent successfully.\n");

			// ---------------------------------------------

		while(1 && quit==0)
		{
			if(quit == 1)
			{
				close(client);
				break;
			}
			
			contor++;
			printf("%i", contor);
    		// copil
    		close(sd);

    		/* s-a realizat conexiunea, se astepta mesajul */
    		bzero (msg, 1000);
			printf ("Welcome to TopMusic - Server\n");
			//printf("%s\n", sqlite3_libversion()); 
    		printf ("Awaiting input from client...\n");
    		fflush (stdout);

    		/* citirea mesajului */
    		if (read (client, msg, 1000) <= 0)
    		{
    			perror ("Eroare la read() de la client.\n");
    			close (client);	/* inchidem conexiunea cu clientul */
    			continue;		/* continuam sa ascultam */
    		}

    		printf (" Input received: %s\n", msg);
			
			// --- FUNCTIE EXIT ------------

			if(strstr(msg,"exit")!=0)
				{
					printf("Ending connection with client...");
					write(client,"Connection terminated.\n",1000);
					quit = 1;
					close(client);
					break;
				}

			// --- FUNCTIE REGISTER --------

			if(strstr(msg,"register")!=0)
				{
					registration(msg);
				}

			// -----------------------------

    		/*pregatim mesajul de raspuns */
    		bzero(msgrasp,1000);
    		strcat(msgrasp,"Hello ");
    		strcat(msgrasp,msg);

    		printf(" Sending response back to client: %s\n",msgrasp);


    		/* returnam mesajul clientului */
    		if (write (client, msgrasp, 1000) <= 0)
    		{
    			perror ("Eroare la write() catre client.\n");
    			continue;		/* continuam sa ascultam */
    		}
    		else
    			printf (" Response sent successfully.\n");


    	}
		exit(0);//while
	} // else if

    }				/* while */
}				/* main */