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
int voteFlag = 0;
#define BUF 10000

void menu(int nl)
{
  if (nl == 1)
  {
    printf("\n \n \n");
  }
  // Welcome Message
  if (loginFlag == 0)
    printf(" Welcome to TopMusic!\n Please enter one of the following commands:\n \n 1) register\n 2) login\n 3) help\n 4) exit \n \n");
  else if (loginFlag == 1 && adminFlag == 0)
    printf(" Welcome to TopMusic!\n You are logged in as user.\n Please enter one of the following commands:\n \n 1) addSong              7) topByVotes \n 2) showSongs            8) topByGenre \n 3) showGenres           9) addComment \n 4) showDescription     10) showComments \n 5) vote                11) help \n 6) openLink            12) exit \n \n");
  else if (loginFlag == 1 && adminFlag == 1)
    printf(" Welcome to TopMusic!\n You are logged in as admin.\n Please enter one of the following commands:\n \n 1) addSong               11) addComment \n 2) showSongs             12) showComments \n 3) deleteSong            13) deleteComment \n 4) addGenre              14) topByGenre \n 5) showGenres            15) topByVotes \n 6) deleteGenre           16) showUsers \n 7) addDescription        17) setVoteRight \n 8) showDescription       18) vote \n 9) deleteDescription     19) help \n10) openLink              20) exit \n \n");
}

int main(int argc, char *argv[])
{
  int sd;
  struct sockaddr_in server;
  char msg[BUF];

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

  system("clear");
  menu(0);
  while (1)
  {

    bzero(msg, BUF);
    read(0, msg, sizeof(msg)); // citire de comanda de la tastatura
    msg[strlen(msg) - 1] = 0;

    if (strcmp(msg, "register") == 0 || ((loginFlag == 0) && (strcmp(msg, "1") == 0)))
    {
      if (loginFlag == 0)
      {
        write(sd, "register", sizeof(msg)); //scrie register la server
        bzero(msg, BUF);
        printf("Please provide an username:\n");
        read(0, msg, sizeof(msg)); // citeste de la tastatura username
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie la server username-ul (1)
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste valid/invalid de la server; (2)
        if (strcmp(msg, "valid") == 0)
        {
          bzero(msg, BUF);
          printf("Please provide a password:\n");
          read(0, msg, sizeof(msg)); // citire de la tastatura
          msg[strlen(msg) - 1] = 0;
          write(sd, msg, sizeof(msg)); // scrie la server parola (3)
          bzero(msg, BUF);
          printf("Registration successful.\n");
          sleep(3);
          menu(1);
        }
        else
        {
          bzero(msg, BUF);
          printf("This username is taken. Try again.\n");
        }
      }
      else
        printf("You are already logged in. You must be signed out to perform this action.\n");
    }

    else if (strcmp(msg, "login") == 0 || ((loginFlag == 0) && (strcmp(msg, "2") == 0)))
    {
      if (loginFlag == 0)
      {
        write(sd, "login", sizeof(msg)); // scrie login la server
        bzero(msg, BUF);
        printf("Enter your username:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie username la server (1)
        bzero(msg, BUF);
        printf("Enter your password:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie parola la server (2)
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste daca e admin sau user de la server (3)
        if (strcmp(msg, "admin") == 0)
        {
          printf("Login as admin successful.\n\n");
          adminFlag = 1;
          loginFlag = 1;
          menu(1);
        }
        else if (strcmp(msg, "user") == 0)
        {
          printf("Login as user successful.\n\n");
          adminFlag = 0;
          loginFlag = 1;
          menu(1);
        }
        else
        {
          printf("Login failed. Please try again.\n");
          adminFlag = 0;
          loginFlag = 0;
          menu(1);
        }
      }
      else
        printf("You are already logged in. You must be signed out to perform this action.\n");
    }

    else if (strcmp(msg, "help") == 0 || ((loginFlag == 0) && (strcmp(msg, "3") == 0)) || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "11") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "19") == 0)))
    {
      menu(0);
    }

    else if (strcmp(msg, "exit") == 0 || ((loginFlag == 0) && (strcmp(msg, "4") == 0)) || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "12") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "20") == 0)))
    {
      write(sd, "exit", sizeof(msg)); //scrie exit la server
      bzero(msg, BUF);
      printf("Connection Terminated.\n");
      break;
    }

    else if (strcmp(msg, "addSong") == 0 || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "1") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "1") == 0)))
    {
      char genreList[BUF];
      if (loginFlag == 1)
      {
        write(sd, "addSong", sizeof(msg)); // scrie addSong la server
        bzero(msg, BUF);
        printf("Enter the song name:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie song_name la server (1)
        bzero(msg, BUF);
        printf("Enter the song artist:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie song_artist la server (2)
        bzero(msg, BUF);
        printf("Enter the song URL:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie link la server (3)
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista de genres de la server (4)
        strcpy(genreList, msg);
        printf("%s\n Select genre1 from these genres:\n ", genreList);
        bzero(msg, BUF);
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie genre1 la server (5)
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg));    // citeste validare genre1 (6)
        if (strcmp(msg, "valid") == 0) //validare genre1
        {
          bzero(msg, BUF);
          printf("Selected genre is valid \n Please enter genre 2 or leave the field blank \n");

          printf("%s\n Select genre2 from these genres:\n ", genreList);
          read(0, msg, sizeof(msg));
          msg[strlen(msg) - 1] = 0;
          write(sd, msg, sizeof(msg)); // scrie genre2 la server (7)
          bzero(msg, BUF);
          read(sd, msg, sizeof(msg));    // citeste validare genre2 (8)
          if (strcmp(msg, "valid") == 0) //validare genre2
          {
            bzero(msg, BUF);
            printf("Selected genre is valid \n Please enter genre 3 or leave the field blank \n");

            printf("%s\n Select genre3 from these genres:\n ", genreList);
            read(0, msg, sizeof(msg));
            msg[strlen(msg) - 1] = 0;
            write(sd, msg, sizeof(msg)); // scrie genre3 la server (9)
            bzero(msg, BUF);
            read(sd, msg, sizeof(msg));    // citeste validare genre3 (10)
            if (strcmp(msg, "valid") == 0) // validare genre3
            {
              printf("Song added successfully.\n");
              sleep(3);
              menu(1);
            }
            else // eroare la genre3
            {
              printf("Song was not added.  %s. Please try again.\n", msg);
              bzero(msg, BUF);
            }
          }
          else // eroare la genre2
          {
            printf("Song was not added.  %s. Please try again.\n", msg);
            bzero(msg, BUF);
          }
        }
        else // eroare la genre1
        {
          printf("Song was not added. %s. Please try again.\n", msg);
          bzero(msg, BUF);
        }
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "showSongs") == 0 || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "2") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "2") == 0)))
    {
      if (loginFlag == 1)
      {
        write(sd, "showSongs", sizeof(msg)); // scrie showSongs la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg));
        printf("\n%s\n", msg);
        bzero(msg, BUF);
        sleep(3);
        menu(0);
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "deleteSong") == 0 || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "3") == 0)))
    {
      if (loginFlag == 1 && adminFlag == 1)
      {
        write(sd, "deleteSong", sizeof(msg)); // scrie deleteSong la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista de melodii de la server (1)
        printf("%s \nEnter the ID of the song you want to delete. \n", msg);
        bzero(msg, BUF);
        read(0, msg, sizeof(msg)); // citeste de la tastatura ID
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie ID la server (2)
        printf("Song deleted successfully.\n");
        sleep(3);
        menu(1);
      }
      else
        printf("You must be logged in as admin to perform this action.\n");
    }

    else if (strcmp(msg, "addGenre") == 0 || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "4") == 0)))
    {
      if (loginFlag == 1 && adminFlag == 1)
      {
        write(sd, msg, sizeof(msg)); // scrie addGenre la server
        bzero(msg, BUF);
        printf("Enter the genre name:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie genre_name la server (1)
        bzero(msg, BUF);
        printf("Enter a genre description (optional):\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie descriere la server (2)
        bzero(msg, BUF);
        printf("Genre added successfully.\n");
        sleep(3);
        menu(1);
      }
      else
        printf("You must be logged in as admin to perform this action.\n");
    }

    else if (strcmp(msg, "showGenres") == 0 || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "3") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "5") == 0)))
    {
      if (loginFlag == 1)
      {
        write(sd, "showGenres", sizeof(msg)); // scrie showGenres la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg));
        printf("%s \n", msg);
        bzero(msg, BUF);
        sleep(3);
        menu(0);
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "deleteGenre") == 0 || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "6") == 0)))
    {
      if (loginFlag == 1 && adminFlag == 1)
      {
        write(sd, "deleteGenre", sizeof(msg)); // scrie deleteGenre la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista de melodii de la server (1)
        printf("%s \nEnter the ID of the genre you want to delete. \n", msg);
        bzero(msg, BUF);
        read(0, msg, sizeof(msg)); // citeste de la tastatura ID
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie ID la server (2)
        printf("Genre deleted successfully.\n");
        sleep(3);
        menu(1);
      }
      else
        printf("You must be logged in as admin to perform this action.\n");
    }

    else if (strcmp(msg, "addDescription") == 0 || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "7") == 0)))
    {
      if (loginFlag == 1 && adminFlag == 1)
      {
        write(sd, "addDescription", sizeof(msg)); // scrie addDescription la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista de melodii de la server (1)
        printf("%s \nEnter the ID of the song you want to add a description for. \n", msg);
        bzero(msg, BUF);

        read(0, msg, sizeof(msg)); // citeste de la tastatura ID
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie ID la server (2)

        printf("Enter the description for song with ID%s. \n", msg);
        bzero(msg, BUF);
        read(0, msg, sizeof(msg)); // citeste de la tastatura descriere
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie descriere la server (3)
        printf("Song description added successfully.\n");
        sleep(3);
        menu(1);
      }
      else
        printf("You must be logged in as admin to perform this action.\n");
    }

    else if (strcmp(msg, "showDescription") == 0 || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "4") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "8") == 0)))
    {
      if (loginFlag == 1)
      {
        write(sd, "showDescription", sizeof(msg)); // scrie vote la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista de melodii de la server (1)
        printf("%s \nEnter the ID of the song you want to see the description of. \n", msg);
        bzero(msg, BUF);
        read(0, msg, sizeof(msg)); // citeste de la tastatura ID
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie ID la server (2)
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste descrierea (3)
        if (strlen(msg) <= 2)
          printf("This song has no description.\n");
        else
          printf("Description:\n \n %s \n \n", msg);
        bzero(msg, BUF);
        sleep(3);
        menu(1);
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "deleteDescription") == 0 || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "9") == 0)))
    {
      if (loginFlag == 1 && adminFlag == 1)
      {
        write(sd, "deleteDescription", sizeof(msg)); // scrie deleteDescription la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista de melodii de la server (1)
        printf("%s \nEnter the ID of the song you want to delete the description for. \n", msg);
        bzero(msg, BUF);
        read(0, msg, sizeof(msg)); // citeste de la tastatura ID
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie ID la server (2)
        printf("Song description deleted successfully.\n");
        sleep(3);
        menu(1);
      }
      else
        printf("You must be logged in as admin to perform this action.\n");
    }

    else if (strcmp(msg, "addComment") == 0 || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "9") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "11") == 0)))
    {
      if (loginFlag == 1)
      {
        write(sd, "addComment", sizeof(msg)); // scrie comment la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista de melodii de la server (1)
        printf("%s \n", msg);
        bzero(msg, BUF);
        printf("Select a Song ID to comment on:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie SongID la server (2)
        bzero(msg, BUF);
        printf("Your Comment:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie comment la server (3)
        bzero(msg, BUF);
        printf("\n Comment added successfully.\n");
        sleep(3);
        menu(1);
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "showComments") == 0 || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "10") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "12") == 0)))
    {
      if (loginFlag == 1)
      {
        write(sd, "showComments", sizeof(msg)); // scrie comment la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista de melodii de la server (1)
        printf("%s \n", msg);
        bzero(msg, BUF);
        printf("Select a Song ID to see comments for:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie SongID la server (2)
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste comentariile (3)
        printf("\n %s \n", msg);
        sleep(3);
        menu(0);
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "deleteComment") == 0 || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "13") == 0)))
    {
      if (loginFlag == 1 && adminFlag == 1)
      {
        write(sd, "deleteComment", sizeof(msg)); // scrie comment la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista de melodii de la server (1)
        printf("%s \n", msg);
        bzero(msg, BUF);
        printf("Select a Comment ID to delete:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie commentID la server (2)
        bzero(msg, BUF);
        printf("Comment deleted successfully.\n");
        sleep(3);
        menu(1);
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "openLink") == 0 || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "6") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "10") == 0)))
    {
      if (loginFlag == 1)
      {
        write(sd, "openLink", sizeof(msg)); // scrie openLink la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista de melodii de la server (1)
        printf("%s \nEnter the ID of the song you want to open the link of. \n", msg);
        bzero(msg, BUF);
        read(0, msg, sizeof(msg)); // citeste de la tastatura ID
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie ID la server (2)
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste URL (3)
        if (strlen(msg) <= 2)
          printf("This song has no URL.\n");
        else
        {
          char linkcommand[BUF];
          snprintf(linkcommand, sizeof(linkcommand), "python -mwebbrowser %s", msg);
          system(linkcommand);
          printf("URL Opened: %s \n", msg);
        }
        bzero(msg, BUF);
        sleep(3);
        menu(1);
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "topByGenre") == 0 || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "8") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "14") == 0)))
    {
      if (loginFlag == 1)
      {
        write(sd, "topByGenre", sizeof(msg)); // scrie topByVotes la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste de la client genres; (1)
        printf("%s \n", msg);
        bzero(msg, BUF);
        printf("Enter the genre name:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie genre_name la server (2)
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste de la server songs (3)
        printf("\n %s \n", msg);
        bzero(msg, BUF);
        sleep(3);
        menu(0);
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "topByVotes") == 0 || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "7") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "15") == 0)))
    {
      if (loginFlag == 1)
      {
        write(sd, "topByVotes", sizeof(msg)); // scrie topByVotes la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg));
        printf("\n%s\n", msg);
        bzero(msg, BUF);
        sleep(3);
        menu(0);
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "showUsers") == 0 || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "16") == 0)))
    {
      if (loginFlag == 1 && adminFlag == 1)
      {
        write(sd, "showUsers", sizeof(msg)); // scrie showGenres la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista users de la server
        printf("%s \n", msg);
        bzero(msg, BUF);
        sleep(3);
        menu(0);
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else if (strcmp(msg, "setVoteRight") == 0 || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "17") == 0)))
    {
      if (loginFlag == 1 && adminFlag == 1)
      {
        write(sd, "setVoteRight", sizeof(msg)); // scrie setVoteRight la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste lista users de la server (1)
        printf("%s \n", msg);
        bzero(msg, BUF);
        printf("Select an ID to change voting rights:\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie userID la server (2)
        bzero(msg, BUF);
        printf("To allow the user to vote, write 1.\n To deny the right to vote, write 0.\n");
        read(0, msg, sizeof(msg));
        msg[strlen(msg) - 1] = 0;
        write(sd, msg, sizeof(msg)); // scrie 1 sau 0 la server (3)
        if (strcmp(msg, "1") == 0)
          printf("User can vote.\n");
        else
          printf("User cannot vote.\n");
        menu(1);
      }
      else
        printf("You must be logged in as admin to perform this action.\n");
    }

    else if (strcmp(msg, "vote") == 0 || ((loginFlag == 1) && (adminFlag == 0) && (strcmp(msg, "5") == 0)) || ((loginFlag == 1) && (adminFlag == 1) && (strcmp(msg, "18") == 0)))
    {
      if (loginFlag == 1)
      {
        write(sd, "vote", sizeof(msg)); // scrie vote la server
        bzero(msg, BUF);
        read(sd, msg, sizeof(msg)); // citeste validare drept de vot (1)
        voteFlag = atoi(msg);
        if (voteFlag == 1 || adminFlag == 1)
        {
          read(sd, msg, sizeof(msg)); // citeste lista de melodii de la server (2)
          printf("%s \nEnter the ID of the song you want to vote. \n", msg);
          bzero(msg, BUF);
          read(0, msg, sizeof(msg)); // citeste de la tastatura ID
          msg[strlen(msg) - 1] = 0;
          write(sd, msg, sizeof(msg)); // scrie ID la server (3)
          bzero(msg, BUF);
          printf("Song was voted successfully.\n");
          sleep(3);
          menu(1);
        }
        else
          printf("You don't have permission to vote\n");
      }
      else
        printf("You must be logged in to perform this action.\n");
    }

    else
      printf("Unknown command. Type \"help\" to list all the available commands.\n");
  }

  close(sd);
}