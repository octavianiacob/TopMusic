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
int loginFlag = 0;
int adminFlag = 0;

void menu()
{
  // Welcome Message
  if (loginFlag == 0)
    printf("Welcome to TopMusic!\n Please enter one of the following commands:\n 1) register\n 2) login\n 3) exit\n 4) commands\n");
  else if (loginFlag == 1 && adminFlag == 0)
    printf("Welcome to TopMusic!\n You are logged in as user.\n Please enter one of the following commands:\n 1) addSong\n 2) vote\n 3) exit\n 4) commands\n");
  else if (loginFlag == 1 && adminFlag == 1)
    printf("Welcome to TopMusic!\n You are logged in as admin.\n Please enter one of the following commands:\n 1) command1\n 2) command2\n 3) exit\n 4) commands\n");
}

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

  menu();
  while (1)
  {

    bzero(msg, 1000);
    read(0, msg, sizeof(msg)); // citire de comanda de la tastatura
    msg[strlen(msg) - 1] = 0;
    //-------------------------------------------------------------

    if (strcmp(msg, "register") == 0)
    {
      if (loginFlag == 0)
      {
        write(sd, msg, sizeof(msg)); //scrie register la server
        bzero(msg, 1000);
        printf("Please provide an username:\n");
        read(0, msg, sizeof(msg)); // citeste de la tastatura username
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie la server username-ul (1)
        bzero(msg, 1000);
        read(sd, msg, sizeof(msg)); // citeste valid/invalid de la server; (2)
        if (strcmp(msg, "valid") == 0)
        {
          bzero(msg, 1000);
          printf("Please provide a password:\n");
          read(0, msg, sizeof(msg)); // citire de la tastatura
          msg[strlen(msg) - 1] = 0;
          write(sd, msg, sizeof(msg)); // scrie la server parola (3)
          bzero(msg, 1000);
          printf("Registration successful.\n");
        }
        else
        {
          bzero(msg, 1000);
          printf("This username is taken. Try again.\n");
        }
      }
      else
        printf("You are already logged in. You must be signed out to perform this action.\n");
    }

    else if (strcmp(msg, "login") == 0)
    {
      if (loginFlag == 0)
      {
        write(sd, msg, sizeof(msg)); // scrie login la server
        bzero(msg, 1000);
        printf("Enter your username:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie username la server (1)
        bzero(msg, 1000);
        printf("Enter your password:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie parola la server (2)
        bzero(msg, 1000);
        read(sd, msg, sizeof(msg)); // citeste daca e admin sau user de la server (3)
        if (strcmp(msg, "admin") == 0)
        {
          printf("Login as admin successful.\n");
          adminFlag = 1;
          loginFlag = 1;
        }
        else if (strcmp(msg, "user") == 0)
        {
          printf("Login as user successful.\n");
          adminFlag = 0;
          loginFlag = 1;
        }
        else
        {
          printf("Login failed. Please try again.\n");
          adminFlag = 0;
          loginFlag = 0;
        }
      }
      else
        printf("You are already logged in. You must be signed out to perform this action.\n");
    }

    else if (strcmp(msg, "exit") == 0)
    {
      write(sd, msg, sizeof(msg)); //scrie exit la server
      bzero(msg, 1000);
      printf("Connection Terminated.\n");
      break;
    }

    else if (strcmp(msg, "commands") == 0)
    {
      menu();
    }
  }

  close(sd);
}