/* cliTCPIt.c - Exemplu de client TCP
   Trimite un nume la server; primeste de la server "Hello nume".
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main(int argc, char *argv[])
{
  int sd;                    // descriptorul de socket
  struct sockaddr_in server; // structura folosita pentru conectare
  char msg[1000];            // mesajul trimis

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
  {
    printf("Sintax: %s <server_address> <port>\n", argv[0]);
    return -1;
  }

  /* stabilim portul */
  port = atoi(argv[2]);

  /* cream socketul */
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Eroare la socket().\n");
    return errno;
  }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons(port);

  /* ne conectam la server */
  if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[client]Eroare la connect().\n");
    return errno;
  }

  // Welcome Message

  printf("%s", "Welcome to TopMusic!\n Please enter one of the following commands:\n 1) register\n 2) login\n 3) exit\n");

  while (1)
  {
    bzero(msg, 1000);
    read(0, msg, sizeof(msg)); // citire de comanda de la tastatura
    msg[strlen(msg) - 1] = 0;
    //-------------------------------------------------------------
    if (strcmp(msg, "register") == 0)
    {
      write(sd, msg, sizeof(msg));//scrie register la server
      bzero(msg, 1000);
      printf("Please provide an username:\n");
      read(0, msg, sizeof(msg)); // citeste de la tastatura username
      msg[strlen(msg) - 1] = 0;
      write(sd, msg, sizeof(msg)); // scrie la server username-ul
      bzero(msg, 1000);
      printf("Please provide a password:\n");
      read(0, msg, sizeof(msg)); // citire de la tastatura
      msg[strlen(msg) - 1] = 0;
      write(sd, msg, sizeof(msg)); // scrie la server parola
      bzero(msg, 1000);
      printf("Registration successful.\n");

      /*read(sd, msg, sizeof(msg)); // citeste valid/invalid de la server;
      
      if(strcmp(msg,"valid")==0)
      {
      bzero(msg, 1000);
      
      }
      else
        {
          bzero(msg, 1000);
          printf("This username is taken. Try again.\n");
        } */


      
    }
    else if (strcmp(msg, "login") == 0)
    {
      write(sd, msg, sizeof(msg));
      bzero(msg, 1000);
      printf("Enter your username:\n");
      read(0, msg, sizeof(msg)); // citire de la tastatura
      msg[strlen(msg) - 1] = 0;
      write(sd, msg, sizeof(msg));
      bzero(msg, 1000);
      printf("Enter your password:\n");
      read(0, msg, sizeof(msg)); // citire de la tastatura
      msg[strlen(msg) - 1] = 0;
      printf("Login successful.\n");
    }

    write(sd, msg, sizeof(msg)); // scrierea catre server
    bzero(msg, 1000);

    read(sd, msg, sizeof(msg)); // citire mesaj de la server
    printf("%s\n", msg);
  }

  close(sd);
}