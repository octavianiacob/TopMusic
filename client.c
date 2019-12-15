#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

extern int errno;
int port;
int loginFlag = 0;
int adminFlag = 0;

void menu()
{
  // Welcome Message
  if (loginFlag == 0)
    printf("Welcome to TopMusic!\n Please enter one of the following commands:\n 1) register\n 2) login\n 3) exit\n 4) commands\n");
  else if (loginFlag == 1 && adminFlag == 0)
    printf("Welcome to TopMusic!\n You are logged in as user.\n Please enter one of the following commands:\n 1) addSong\n 2) vote\n 3) exit\n 4) commands\n 5) showGenres \n");
  else if (loginFlag == 1 && adminFlag == 1)
    printf("Welcome to TopMusic!\n You are logged in as admin.\n Please enter one of the following commands:\n 1) addSong\n 2) addGenre\n 3) vote\n 4) exit\n 5) commands\n 6) showGenres \n");
}

int main(int argc, char *argv[])
{
  int sd;
  struct sockaddr_in server;
  char msg[1000];

  if (argc != 3)
  {
    printf("Sintax: %s <server_address> <port>\n", argv[0]);
    return -1;
  }

  port = atoi(argv[2]);
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Eroare la socket().\n");
    return errno;
  }
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(port);

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
          printf("Login as admin successful.\n\n");
          adminFlag = 1;
          loginFlag = 1;
          menu();
        }
        else if (strcmp(msg, "user") == 0)
        {
          printf("Login as user successful.\n\n");
          adminFlag = 0;
          loginFlag = 1;
          menu();
        }
        else
        {
          printf("Login failed. Please try again.\n");
          adminFlag = 0;
          loginFlag = 0;
          menu();
        }
      }
      else
        printf("You are already logged in. You must be signed out to perform this action.\n");
    }

    else if (strcmp(msg, "addSong") == 0)
    {
      char genreList[1000];
      if (loginFlag == 1)
      {
        write(sd, msg, sizeof(msg)); // scrie addSong la server
        bzero(msg, 1000);
        printf("Enter the song name:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie song_name la server (1)
        bzero(msg, 1000);
        printf("Enter the song artist:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie song_artist la server (2)
        bzero(msg, 1000);
        printf("Enter the song URL:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie link la server (3)
        bzero(msg, 1000);
        read(sd, msg, sizeof(msg)); // citeste lista de genres de la server (4)
        strcpy(genreList, msg);
        printf("%s\n Select genre1 from these genres:\n ", genreList);
        bzero(msg, 1000);
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie genre1 la server (5)
        bzero(msg, 1000);
        read(sd, msg, sizeof(msg));    // citeste validare genre1 (6)
        if (strcmp(msg, "valid") == 0) //validare genre1
        {
          bzero(msg, 1000);
          printf("Selected genre is valid \n Please enter genre 2 or leave the field blank \n");

          printf("%s\n Select genre2 from these genres:\n ", genreList);
          read(0, msg, sizeof(msg));
          msg[strlen(msg) - 1] = 0;
          write(sd, msg, sizeof(msg)); // scrie genre2 la server (7)
          bzero(msg, 1000);
          read(sd, msg, sizeof(msg));    // citeste validare genre2 (8)
          if (strcmp(msg, "valid") == 0) //validare genre2
          {
            bzero(msg, 1000);
            printf("Selected genre is valid \n Please enter genre 3 or leave the field blank \n");

            printf("%s\n Select genre3 from these genres:\n ", genreList);
            read(0, msg, sizeof(msg));
            msg[strlen(msg) - 1] = 0;
            write(sd, msg, sizeof(msg)); // scrie genre3 la server (9)
            bzero(msg, 1000);
            read(sd, msg, sizeof(msg));    // citeste validare genre3 (10)
            if (strcmp(msg, "valid") == 0) // validare genre3
            {
              printf("Song added successfully.\n");
            }
            else // eroare la genre3
            {
              printf("Song was not added.  %s. Please try again.\n", msg);
              bzero(msg, 1000);
            }
          }
          else // eroare la genre2
          {
            printf("Song was not added.  %s. Please try again.\n", msg);
            bzero(msg, 1000);
          }
        }
        else // eroare la genre1
        {
          printf("Song was not added. %s. Please try again.\n", msg);
          bzero(msg, 1000);
        }
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "addGenre") == 0)
    {
      if (loginFlag == 1 && adminFlag == 1)
      {
        write(sd, msg, sizeof(msg)); // scrie addGenre la server
        bzero(msg, 1000);
        printf("Enter the genre name:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie genre_name la server (1)
        bzero(msg, 1000);
        printf("Enter a genre description (optional):\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie descriere la server (2)
        bzero(msg, 1000);
        printf("Genre added successfully.\n");
      }
      else
        printf("You must be logged in as admin to perform this action.\n");
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

    else if (strcmp(msg, "showGenres") == 0)
    {
      if (loginFlag == 1)
      {
        write(sd, msg, sizeof(msg)); // scrie showGenres la server
        bzero(msg, 1000);
        read(sd, msg, sizeof(msg));
        printf("%s--------------------------------\n", msg);
        bzero(msg, 1000);
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else
      printf("Unknown command. Type \"commands\" to list all the available commands.\n");
  }

  close(sd);
}