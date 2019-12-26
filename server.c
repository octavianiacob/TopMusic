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
#define BUF 10000

extern int errno;

char username[BUF], password[BUF];
int loginFlag, adminFlag, voteFlag, ID;
sqlite3 *database;
sqlite3_stmt *statement;
char *query = NULL;
char *err_msg = 0;
char str[BUF];
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
	char isValid[BUF];
	bzero(isValid, BUF);
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

void insertSong(char song_name[], char song_artist[], char song_link[], char genre1[], char genre2[], char genre3[])
{
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database.\n");
	else
		printf("Database opened successfully.\n");
	asprintf(&query, "insert into songs (song_name, song_artist, link, genre1, genre2, genre3, votes, song_description) values (\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\", 0, \"\");", song_name, song_artist, song_link, genre1, genre2, genre3);
	//asprintf(&query, "insert into songs (song_name, song_artist, link, genre1) values (\"%s\", \"%s\", \"%s\", \"%s\");", song_name, song_artist, song_link);
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

void showGenres(char *genres)
{
	int genre_count = 0;
	char genresDisplay[BUF];
	bzero(genresDisplay, BUF);
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
	strcat(genresDisplay, "\n  ID      Genre      Description\n--------------------------------\n");
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
	strcat(genresDisplay, "--------------------------------\n");
	strcpy(genres, genresDisplay);
}

int isValidGenre(char genre[])
{
	if (genre == "")
		return 1;
	char isValid[BUF];
	bzero(isValid, BUF);
	isValid[0] = '0';
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM genres WHERE genre_name = \"%s\";", genre);
	printf("The following SQL Query will run: '%s'\n", query);
	rc = sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc == SQLITE_ROW)
		sprintf(isValid, sqlite3_column_text(statement, 1));
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);

	if (strcmp(isValid, "0") == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void showSongs(char *songs)
{
	int song_count = 0;
	int spaces;
	char songsDisplay[10000], id[BUF], title[BUF], artist[BUF], link[BUF], votes[BUF], genre1[BUF], genre2[BUF], genre3[BUF];
	bzero(songsDisplay, 10000);
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM songs;");
	printf("The following SQL Query will run: '%s'\n", query);
	if (sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL) != SQLITE_OK)
	{
		sqlite3_close(database);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(database));
	}
	strcat(songsDisplay, "ID    Title               Artist                Link            Votes           Genre 1        Genre 2        Genre 3\n----------------------------------------------------------------------------------------------------------------------------\n");
	while (sqlite3_step(statement) == SQLITE_ROW)
	{
		strcpy(id, sqlite3_column_text(statement, 0));
		spaces = 5 - strlen(id);
		strcat(songsDisplay, id);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		char temptitle[BUF];
		strcpy(temptitle, sqlite3_column_text(statement, 1));
		if (strlen(temptitle) <= 15)
		{
			strcpy(title, temptitle);
			spaces = 20 - strlen(title);
			strcat(songsDisplay, title);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(temptitle, BUF);
			bzero(title, BUF);
		}
		else
		{
			strncpy(title, temptitle, 13);
			strcat(title, "...");
			spaces = 20 - strlen(title);
			strcat(songsDisplay, title);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(temptitle, BUF);
			bzero(title, BUF);
		}

		char tempartist[BUF];
		strcpy(tempartist, sqlite3_column_text(statement, 2));
		if (strlen(tempartist) <= 15)
		{
			strcpy(artist, tempartist);
			spaces = 20 - strlen(artist);
			strcat(songsDisplay, artist);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(tempartist, BUF);
			bzero(artist, BUF);
		}
		else
		{
			strncpy(artist, tempartist, 13);
			strcat(artist, "...");
			spaces = 20 - strlen(artist);
			strcat(songsDisplay, artist);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(tempartist, BUF);
			bzero(artist, BUF);
		}

		char templink[BUF];
		strcpy(templink, sqlite3_column_text(statement, 3));
		if (strlen(templink) <= 15)
		{
			strcpy(link, templink);
			spaces = 20 - strlen(link);
			strcat(songsDisplay, link);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(templink, BUF);
			bzero(link, BUF);
		}
		else
		{
			strncpy(link, templink, 13);
			strcat(link, "...");
			spaces = 20 - strlen(link);
			strcat(songsDisplay, link);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(templink, BUF);
			bzero(link, BUF);
		}

		strcpy(votes, sqlite3_column_text(statement, 4));
		spaces = 15 - strlen(votes);
		strcat(songsDisplay, votes);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcpy(genre1, sqlite3_column_text(statement, 5));
		spaces = 15 - strlen(genre1);
		strcat(songsDisplay, genre1);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcpy(genre2, sqlite3_column_text(statement, 6));
		spaces = 15 - strlen(genre2);
		strcat(songsDisplay, genre2);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcpy(genre3, sqlite3_column_text(statement, 7));
		spaces = 15 - strlen(genre3);
		strcat(songsDisplay, genre3);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcat(songsDisplay, "\n");

		song_count++;
	}

	strcat(songsDisplay, "----------------------------------------------------------------------------------------------------------------------------\n \n");
	printf("Number of songs: %d\n", song_count);
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	//printf("Variabila songsDisplay are:\n %s\n", songsDisplay);
	strcpy(songs, songsDisplay);
}

void topByVotes(char *songs)
{
	int song_count = 1;
	int spaces;
	char songsDisplay[10000], id[BUF], title[BUF], artist[BUF], link[BUF], votes[BUF], genre1[BUF], genre2[BUF], genre3[BUF];
	bzero(songsDisplay, 10000);
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM songs order by votes desc;");
	printf("The following SQL Query will run: '%s'\n", query);
	if (sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL) != SQLITE_OK)
	{
		sqlite3_close(database);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(database));
	}
	strcat(songsDisplay, "ID    Title               Artist                Link            Votes           Genre 1        Genre 2        Genre 3\n----------------------------------------------------------------------------------------------------------------------------\n");
	while (sqlite3_step(statement) == SQLITE_ROW)
	{
		strcpy(id, sqlite3_column_text(statement, 0));
		spaces = 5 - strlen(id);
		strcat(songsDisplay, id);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		char temptitle[BUF];
		strcpy(temptitle, sqlite3_column_text(statement, 1));
		if (strlen(temptitle) <= 15)
		{
			strcpy(title, temptitle);
			spaces = 20 - strlen(title);
			strcat(songsDisplay, title);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(temptitle, BUF);
			bzero(title, BUF);
		}
		else
		{
			strncpy(title, temptitle, 13);
			strcat(title, "...");
			spaces = 20 - strlen(title);
			strcat(songsDisplay, title);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(temptitle, BUF);
			bzero(title, BUF);
		}

		char tempartist[BUF];
		strcpy(tempartist, sqlite3_column_text(statement, 2));
		if (strlen(tempartist) <= 15)
		{
			strcpy(artist, tempartist);
			spaces = 20 - strlen(artist);
			strcat(songsDisplay, artist);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(tempartist, BUF);
			bzero(artist, BUF);
		}
		else
		{
			strncpy(artist, tempartist, 13);
			strcat(artist, "...");
			spaces = 20 - strlen(artist);
			strcat(songsDisplay, artist);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(tempartist, BUF);
			bzero(artist, BUF);
		}

		char templink[BUF];
		strcpy(templink, sqlite3_column_text(statement, 3));
		if (strlen(templink) <= 15)
		{
			strcpy(link, templink);
			spaces = 20 - strlen(link);
			strcat(songsDisplay, link);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(templink, BUF);
			bzero(link, BUF);
		}
		else
		{
			strncpy(link, templink, 13);
			strcat(link, "...");
			spaces = 20 - strlen(link);
			strcat(songsDisplay, link);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(templink, BUF);
			bzero(link, BUF);
		}

		strcpy(votes, sqlite3_column_text(statement, 4));
		spaces = 15 - strlen(votes);
		strcat(songsDisplay, votes);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcpy(genre1, sqlite3_column_text(statement, 5));
		spaces = 15 - strlen(genre1);
		strcat(songsDisplay, genre1);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcpy(genre2, sqlite3_column_text(statement, 6));
		spaces = 15 - strlen(genre2);
		strcat(songsDisplay, genre2);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcpy(genre3, sqlite3_column_text(statement, 7));
		spaces = 15 - strlen(genre3);
		strcat(songsDisplay, genre3);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcat(songsDisplay, "\n");

		song_count++;
	}

	strcat(songsDisplay, "----------------------------------------------------------------------------------------------------------------------------\n \n");
	printf("Number of songs: %d\n", song_count);
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	//printf("Variabila songsDisplay are:\n %s\n", songsDisplay);
	strcpy(songs, songsDisplay);
}

void topByGenre(char *songs, char *genre)
{
	int song_count = 1;
	int spaces;
	char songsDisplay[10000], id[BUF], title[BUF], artist[BUF], link[BUF], votes[BUF], genre1[BUF], genre2[BUF], genre3[BUF];
	bzero(songsDisplay, 10000);
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "select * from songs where genre1 = \"%s\" or genre2 = \"%s\" or genre3 = \"%s\";", genre, genre, genre);
	printf("The following SQL Query will run: '%s'\n", query);
	if (sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL) != SQLITE_OK)
	{
		sqlite3_close(database);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(database));
	}
	strcat(songsDisplay, "ID    Title               Artist                Link            Votes           Genre 1        Genre 2        Genre 3\n----------------------------------------------------------------------------------------------------------------------------\n");
	while (sqlite3_step(statement) == SQLITE_ROW)
	{
		strcpy(id, sqlite3_column_text(statement, 0));
		spaces = 5 - strlen(id);
		strcat(songsDisplay, id);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		char temptitle[BUF];
		strcpy(temptitle, sqlite3_column_text(statement, 1));
		if (strlen(temptitle) <= 15)
		{
			strcpy(title, temptitle);
			spaces = 20 - strlen(title);
			strcat(songsDisplay, title);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(temptitle, BUF);
			bzero(title, BUF);
		}
		else
		{
			strncpy(title, temptitle, 13);
			strcat(title, "...");
			spaces = 20 - strlen(title);
			strcat(songsDisplay, title);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(temptitle, BUF);
			bzero(title, BUF);
		}

		char tempartist[BUF];
		strcpy(tempartist, sqlite3_column_text(statement, 2));
		if (strlen(tempartist) <= 15)
		{
			strcpy(artist, tempartist);
			spaces = 20 - strlen(artist);
			strcat(songsDisplay, artist);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(tempartist, BUF);
			bzero(artist, BUF);
		}
		else
		{
			strncpy(artist, tempartist, 13);
			strcat(artist, "...");
			spaces = 20 - strlen(artist);
			strcat(songsDisplay, artist);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(tempartist, BUF);
			bzero(artist, BUF);
		}

		char templink[BUF];
		strcpy(templink, sqlite3_column_text(statement, 3));
		if (strlen(templink) <= 15)
		{
			strcpy(link, templink);
			spaces = 20 - strlen(link);
			strcat(songsDisplay, link);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(templink, BUF);
			bzero(link, BUF);
		}
		else
		{
			strncpy(link, templink, 13);
			strcat(link, "...");
			spaces = 20 - strlen(link);
			strcat(songsDisplay, link);
			for (int i = 1; i <= spaces; i++)
				strcat(songsDisplay, " ");
			bzero(templink, BUF);
			bzero(link, BUF);
		}

		strcpy(votes, sqlite3_column_text(statement, 4));
		spaces = 15 - strlen(votes);
		strcat(songsDisplay, votes);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcpy(genre1, sqlite3_column_text(statement, 5));
		spaces = 15 - strlen(genre1);
		strcat(songsDisplay, genre1);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcpy(genre2, sqlite3_column_text(statement, 6));
		spaces = 15 - strlen(genre2);
		strcat(songsDisplay, genre2);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcpy(genre3, sqlite3_column_text(statement, 7));
		spaces = 15 - strlen(genre3);
		strcat(songsDisplay, genre3);
		for (int i = 1; i <= spaces; i++)
			strcat(songsDisplay, " ");

		strcat(songsDisplay, "\n");

		song_count++;
	}

	strcat(songsDisplay, "----------------------------------------------------------------------------------------------------------------------------\n \n");
	printf("Number of songs: %d\n", song_count);
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	//printf("Variabila songsDisplay are:\n %s\n", songsDisplay);
	strcpy(songs, songsDisplay);
}

void voteSong(int songID)
{
	int votes = -1;
	// Citire nr curent de voturi;

	char vv[BUF];
	bzero(vv, BUF);
	vv[0] = '0';
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM songs WHERE id_song = \"%d\";", songID);
	printf("The following SQL Query will run: '%s'\n", query);
	rc = sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc == SQLITE_ROW)
		sprintf(vv, sqlite3_column_text(statement, 4));
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);

	votes = atoi(vv);
	printf("SONG ID: %d  CURRENT NUMBER OF VOTES: %d \n", songID, votes);

	// Modificare votes
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "update songs set votes = \"%d\" where id_song = \"%d\";", votes + 1, songID);
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

void addDescription(int songID, char *description)
{
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "update songs set song_description = \"%s\" where id_song = \"%d\";", description, songID);
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

void showDescription(char *songs, int songID)
{
	char songsDisplay[BUF];
	bzero(songsDisplay, BUF);
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM songs where id_song = %d;", songID);
	printf("The following SQL Query will run: '%s'\n", query);
	if (sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL) != SQLITE_OK)
	{
		sqlite3_close(database);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(database));
	}
	if (sqlite3_step(statement) == SQLITE_ROW)
	{
		strcat(songsDisplay, sqlite3_column_text(statement, 8));
		strcat(songsDisplay, "\n");
	}
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	strcpy(songs, songsDisplay);
}

void getSongURL(int songID, char *URL)
{
	char vv[BUF];
	bzero(vv, BUF);
	vv[0] = '0';
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM songs WHERE id_song = \"%d\";", songID);
	printf("The following SQL Query will run: '%s'\n", query);
	rc = sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc == SQLITE_ROW)
		sprintf(vv, sqlite3_column_text(statement, 3));
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	strcpy(URL, vv);
}

void deleteSong(int songID)
{
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "DELETE FROM songs where id_song = %d;", songID);
	printf("The following SQL Query will run: '%s'\n", query);
	if (sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL) != SQLITE_OK)
	{
		sqlite3_close(database);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(database));
	}
	if (sqlite3_step(statement) == SQLITE_ROW)
	{
		printf("Delete was successful.\n");
	}
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
}

void deleteGenre(int genreID)
{
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "DELETE FROM genres where genre_id = %d;", genreID);
	printf("The following SQL Query will run: '%s'\n", query);
	if (sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL) != SQLITE_OK)
	{
		sqlite3_close(database);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(database));
	}
	if (sqlite3_step(statement) == SQLITE_ROW)
	{
		printf("Delete was successful.\n");
	}
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
}

int getVoteRight(int userID)
{
	char vv[BUF];
	bzero(vv, BUF);
	vv[0] = '0';
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "select * from user where user_id = %d;", userID);
	printf("The following SQL Query will run: '%s'\n", query);
	rc = sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc == SQLITE_ROW)
		sprintf(vv, sqlite3_column_text(statement, 7));
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	return atoi(vv);
}

void setVoteRight(int userID, int canVote)
{

	char vv[BUF];
	bzero(vv, BUF);
	vv[0] = '0';
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "update user set can_vote = \"%d\" where user_id = \"%d\";", canVote, userID);
	printf("The following SQL Query will run: '%s'\n", query);
	rc = sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc == SQLITE_ROW)
		sprintf(vv, sqlite3_column_text(statement, 7));
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	printf("User with ID = %d has vote right = %d \n", userID, canVote);
}

int getUserID(char username[])
{
	char vv[BUF];
	bzero(vv, BUF);
	vv[0] = '0';
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "select * from user where user_name = \"%s\";", username);
	printf("The following SQL Query will run: '%s'\n", query);
	rc = sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc == SQLITE_ROW)
		sprintf(vv, sqlite3_column_text(statement, 0));
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	return atoi(vv);
}

int getAdminID(char username[])
{
	char vv[BUF];
	bzero(vv, BUF);
	vv[0] = '0';
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "select * from admin where admin_name = \"%s\";", username);
	printf("The following SQL Query will run: '%s'\n", query);
	rc = sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL);
	rc = sqlite3_step(statement);
	if (rc == SQLITE_ROW)
		sprintf(vv, sqlite3_column_text(statement, 0));
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	return atoi(vv);
}

void showUsers(char *users)
{
	int user_count = 0;
	char usersDisplay[BUF], id[BUF], username[BUF], vr[BUF];
	int spaces;
	bzero(usersDisplay, BUF);
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM user;");
	printf("The following SQL Query will run: '%s'\n", query);
	if (sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL) != SQLITE_OK)
	{
		sqlite3_close(database);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(database));
	}
	strcat(usersDisplay, "\n  ID   Username        Can Vote\n-----------------------------------\n");
	while (sqlite3_step(statement) == SQLITE_ROW)
	{
		strcat(usersDisplay, "  ");

		strcpy(id, sqlite3_column_text(statement, 0));
		spaces = 5 - strlen(id);
		strcat(usersDisplay, id);
		for (int i = 1; i <= spaces; i++)
			strcat(usersDisplay, " ");

		strcpy(username, sqlite3_column_text(statement, 1));
		spaces = 20 - strlen(username);
		strcat(usersDisplay, username);
		for (int i = 1; i <= spaces; i++)
			strcat(usersDisplay, " ");

		strcpy(vr, sqlite3_column_text(statement, 7));
		spaces = 20 - strlen(vr);
		strcat(usersDisplay, vr);
		for (int i = 1; i <= spaces; i++)
			strcat(usersDisplay, " ");

		strcat(usersDisplay, "\n");
		user_count++;
	}
	printf("Number of genres: %d\n", user_count);
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	//printf("Variabila usersDisplay are:\n %s\n", usersDisplay);
	strcat(usersDisplay, "-----------------------------------\n");
	strcpy(users, usersDisplay);
}

void writeComment(int songID, int userID, char *comment)
{
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "insert into comments (song_id, user_id, comment) values (%d, %d, \"%s\");", songID, userID, comment);
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

void showComments(int songID, char *comments)
{
	int comment_count = 0;
	char commentsDisplay[BUF];
	bzero(commentsDisplay, BUF);
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM comments;");
	printf("The following SQL Query will run: '%s'\n", query);
	if (sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL) != SQLITE_OK)
	{
		sqlite3_close(database);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(database));
	}
	while (sqlite3_step(statement) == SQLITE_ROW)
	{
		strcat(commentsDisplay, "  ");
		strcat(commentsDisplay, sqlite3_column_text(statement, 3));
		strcat(commentsDisplay, "\n");
		comment_count++;
	}
	printf("Number of comments: %d\n", comment_count);
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	//printf("Variabila genresDisplay are:\n %s\n", genresDisplay);
	strcpy(comments, commentsDisplay);
}

void showAllComments(char *comments)
{
	int comment_count = 0;
	char commentsDisplay[BUF], commentID[BUF], songID[BUF], userID[BUF], comment[BUF];
	int spaces;
	bzero(commentsDisplay, BUF);
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "SELECT * FROM comments;");
	printf("The following SQL Query will run: '%s'\n", query);
	if (sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL) != SQLITE_OK)
	{
		sqlite3_close(database);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(database));
	}
	strcat(commentsDisplay, "\n  Comment ID      Song ID      User ID     Comment\n-----------------------------------------------------\n");
	while (sqlite3_step(statement) == SQLITE_ROW)
	{
		strcat(commentsDisplay, "     ");
		strcpy(commentID, sqlite3_column_text(statement, 0));
		spaces = (15 - strlen(commentID));
		strcat(commentsDisplay, commentID);
		for (int i = 1; i <= spaces; i++)
			strcat(commentsDisplay, " ");
		bzero(commentID, BUF);

		strcpy(songID, sqlite3_column_text(statement, 1));
		spaces = (12 - strlen(songID));
		strcat(commentsDisplay, songID);
		for (int i = 1; i <= spaces; i++)
			strcat(commentsDisplay, " ");
		bzero(songID, BUF);

		strcpy(userID, sqlite3_column_text(statement, 2));
		spaces = (10 - strlen(userID));
		strcat(commentsDisplay, userID);
		for (int i = 1; i <= spaces; i++)
			strcat(commentsDisplay, " ");
		bzero(userID, BUF);

		strcat(commentsDisplay, sqlite3_column_text(statement, 3));
		strcat(commentsDisplay, "\n");
		comment_count++;
	}
	printf("Number of comments: %d\n", comment_count);
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
	//printf("Variabila genresDisplay are:\n %s\n", genresDisplay);
	strcpy(comments, commentsDisplay);
}

void deleteComment(int commentID)
{
	rc = sqlite3_open("topmusic.db", &database);
	if (rc)
		printf("Error opening database. \n");
	else
		printf("Database opened successfully. \n");
	asprintf(&query, "DELETE FROM comments where comment_id = %d;", commentID);
	printf("The following SQL Query will run: '%s'\n", query);
	if (sqlite3_prepare_v2(database, query, strlen(query), &statement, NULL) != SQLITE_OK)
	{
		sqlite3_close(database);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(database));
	}
	if (sqlite3_step(statement) == SQLITE_ROW)
	{
		printf("Delete was successful.\n");
	}
	sqlite3_finalize(statement);
	free(query);
	sqlite3_close(database);
}

int main()
{
	struct sockaddr_in server;
	struct sockaddr_in from;
	char input[BUF];		//mesajul primit de la client
	char output[BUF] = " "; //mesaj de raspuns pentru client
	int sd;
	int opt = 1;
	char user_input[BUF]; // copie a mesajului primit de la client

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Error on socket().\n");
		return errno;
	}

	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("Error on setsockopt()");
		exit(EXIT_FAILURE);
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
				if (loginFlag == 1)
				{
					if (adminFlag == 1)
						ID = getAdminID(username);
					else
					{
						ID = getUserID(username);
						voteFlag = getVoteRight(ID);
					}
				}

				close(sd);
				bzero(input, BUF);
				printf("Welcome to TopMusic - Server\n");
				printf("Awaiting input from client...\n");
				printf("LOGIN FLAG = %d , ADMIN FLAG = %d , VOTE FLAG = %d, USER ID = %d\n", loginFlag, adminFlag, voteFlag, ID);
				fflush(stdout);

				if (read(client, input, BUF) <= 0)
				{
					perror("Error on read() from client.\n");
					close(client);
					continue;
				}

				printf(" Input received: %s\n", input);

				if (strcmp(input, "register") == 0)
				{
					bzero(username, BUF);
					bzero(password, BUF);
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

				else if (strcmp(input, "login") == 0)
				{
					bzero(username, BUF);
					bzero(password, BUF);
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

				else if (strcmp(input, "exit") == 0)
				{
					printf("Ending connection with client...\n");
					//write(client, "Connection terminated.\n", BUF);
					close(client);
					exit(0);
				}

				else if (strcmp(input, "addSong") == 0)
				{
					char song_name[BUF], song_artist[BUF], song_link[BUF], genre1[BUF], genre2[BUF], genre3[BUF];

					read(client, song_name, BUF); // citeste song_name de la client (1);
					printf("Song name is: %s \n", song_name);
					read(client, song_artist, BUF); // citeste song_artist de la client (2);
					printf("Song artist is: %s \n", song_artist);
					read(client, song_link, BUF); // citeste link de la client (3);
					printf("link is: %s \n", song_link);

					char genres[BUF];
					showGenres(genres);
					write(client, genres, BUF); // trimite la client genres (4)

					read(client, genre1, BUF); // citeste genre1 de la client (5)
					printf("Song genre1 is: %s \n", genre1);
					if (isValidGenre(genre1) == 1)
					{
						bzero(output, BUF);
						strcat(output, "valid");
						write(client, output, BUF); // scrie valid la client pt genre1 (6)
						bzero(output, BUF);

						read(client, genre2, BUF); // citeste genre2 de la client (7)
						printf("Song genre2 is: %s \n", genre2);

						if (isValidGenre(genre2) == 1 || strcmp(genre2, "") == 0)
						{
							bzero(output, BUF);
							strcat(output, "valid");
							write(client, output, BUF); // scrie valid la client pt genre2 (8)
							bzero(output, BUF);

							read(client, genre3, BUF); // citeste genre3 de la client (9)
							printf("Song genre3 is: %s \n", genre3);

							if (isValidGenre(genre3) == 1 || strcmp(genre3, "") == 0)
							{
								bzero(output, BUF);
								strcat(output, "valid");
								write(client, output, BUF); // scrie valid la client pt genre2 (10)
								bzero(output, BUF);
								printf("The genres are: %s , %s, %s \n", genre1, genre2, genre3);
								insertSong(song_name, song_artist, song_link, genre1, genre2, genre3);
								printf("Song inserted successfully.\n");
							}
							else // eroare la genre3
							{
								bzero(output, BUF);
								strcat(output, "Invalid at genre 3");
								write(client, output, BUF); // scrie invalid la client pt genre3 (10)
								bzero(output, BUF);
							}
						}
						else // eroare la genre2
						{
							bzero(output, BUF);
							strcat(output, "Invalid at genre 2");
							write(client, output, BUF); // scrie invalid la client pt genre2 (8)
							bzero(output, BUF);
						}
					}
					else // eroare la genre1
					{
						bzero(output, BUF);
						strcat(output, "Invalid at genre 1");
						write(client, output, BUF); // scrie invalid la client pt genre1 (6)
						bzero(output, BUF);
					}
				}

				else if (strcmp(input, "showSongs") == 0)
				{
					char songs[BUF];
					showSongs(songs);
					printf("Songs:\n \n %s\n \n", songs);
					write(client, songs, BUF); // trimite la client genres
				}

				else if (strcmp(input, "deleteSong") == 0)
				{
					int songID;
					char ID_as_string[BUF];
					char songs[BUF];
					showSongs(songs);
					write(client, songs, BUF);		 // trimite la client lista de cantece (1)
					read(client, ID_as_string, BUF); // citeste de la client songID (2)
					songID = atoi(ID_as_string);
					deleteSong(songID);
					printf("Deleted song with ID = %d.\n", songID);
				}

				else if (strcmp(input, "addGenre") == 0)
				{
					char genre_name[BUF], genre_description[BUF];
					read(client, genre_name, BUF); // citeste genre name de la client (1);
					printf("Song name is: %s \n", genre_name);
					read(client, genre_description, BUF); // citeste genre description de la client (2);
					printf("Song artist is: %s \n", genre_description);
					if (strcmp(genre_description, "") == 0)
						strcat(genre_description, " ");
					insertGenre(genre_name, genre_description);
					printf("Genre inserted successfully.\n");
				}

				else if (strcmp(input, "showGenres") == 0)
				{
					char genres[BUF];
					showGenres(genres);
					printf("Genres: %s\n \n", genres);
					write(client, genres, BUF); // trimite la client genres
				}

				else if (strcmp(input, "deleteGenre") == 0)
				{
					int genreID;
					char ID_as_string[BUF];
					char genres[BUF];
					showGenres(genres);
					write(client, genres, BUF);		 // trimite la client lista de genuri (1)
					read(client, ID_as_string, BUF); // citeste de la client genreID (2)
					genreID = atoi(ID_as_string);
					deleteGenre(genreID);
					printf("Deleted genre with ID = %d.\n", genreID);
				}

				else if (strcmp(input, "addDescription") == 0)
				{
					int songID;
					char ID_as_string[BUF];
					char songs[BUF];
					char description[BUF];
					showSongs(songs);
					write(client, songs, BUF);		 // trimite la client lista de cantece (1)
					read(client, ID_as_string, BUF); // citeste de la client songID (2)
					read(client, description, BUF);  // citeste de la client descriere (3)
					songID = atoi(ID_as_string);
					addDescription(songID, description);
					printf("Description \"%s\" was added to song with ID = %d.", description, songID);
				}

				else if (strcmp(input, "showDescription") == 0)
				{
					int songID;
					char ID_as_string[BUF];
					char songs[BUF];
					char desc[BUF];
					showSongs(songs);
					write(client, songs, BUF);		 // trimite la client lista de cantece (1)
					read(client, ID_as_string, BUF); // citeste de la client songID (2)
					songID = atoi(ID_as_string);
					showDescription(desc, songID);
					write(client, desc, BUF); // trimite la client descrierea (3)
					printf("Song with ID = %d has the description:\n %s \n.", songID, desc);
				}

				else if (strcmp(input, "deleteDescription") == 0)
				{
					int songID;
					char ID_as_string[BUF];
					char songs[BUF];
					char description[BUF];
					strcpy(description, "");
					showSongs(songs);
					write(client, songs, BUF);		 // trimite la client lista de cantece (1)
					read(client, ID_as_string, BUF); // citeste de la client songID (2)
					songID = atoi(ID_as_string);
					addDescription(songID, description);
					printf("Description was deleted to song with ID = %d.", description, songID);
				}

				else if (strcmp(input, "openLink") == 0)
				{
					char URL[BUF];
					int songID;
					char ID_as_string[BUF];
					char songs[BUF];
					showSongs(songs);
					write(client, songs, BUF);		 // trimite la client lista de cantece (1)
					read(client, ID_as_string, BUF); // citeste de la client songID (2)
					songID = atoi(ID_as_string);
					getSongURL(songID, URL);
					write(client, URL, BUF); // trimite la client URL (3)
				}

				else if (strcmp(input, "writeComment") == 0)
				{
					char songs[BUF], ID_as_string[BUF], comment[BUF], commentWithUsername[BUF];
					int songID;
					int userID;
					sprintf(commentWithUsername, "%s: ", username);
					userID = getUserID(username);
					if (userID == 0)
					{
						userID = getAdminID(username);
						sprintf(commentWithUsername, "[ADMIN]%s: ", username);
					}
					showSongs(songs);
					write(client, songs, BUF);		 // trimite lista melodii (1)
					read(client, ID_as_string, BUF); // citeste songID de la client (2)
					songID = atoi(ID_as_string);
					read(client, comment, BUF); // citeste comment de la client (3)
					strcat(commentWithUsername, comment);
					writeComment(songID, userID, commentWithUsername);
				}

				else if (strcmp(input, "showComments") == 0)
				{
					char comments[BUF], songs[BUF], ID_as_string[BUF];
					int songID;
					write(client, songs, BUF);		 // trimite lista melodii (1)
					read(client, ID_as_string, BUF); // citeste songID de la client (2)
					songID = atoi(ID_as_string);
					showComments(songID, comments);
					write(client, comments, BUF); // trimite comentariile la client (3)
				}

				else if (strcmp(input, "deleteComment") == 0)
				{
					char comments[BUF], ID_as_string[BUF];
					int commentID;
					showAllComments(comments);
					write(client, comments, BUF);	// trimite lista melodii (1)
					read(client, ID_as_string, BUF); // citeste commentID de la client (2)
					commentID = atoi(ID_as_string);
					deleteComment(commentID);
				}

				else if (strcmp(input, "topByGenre") == 0)
				{
					char songs[BUF];
					char selectedGenre[BUF];
					char genres[BUF];
					showGenres(genres);
					write(client, genres, BUF);		  // trimite la client genres (1)
					read(client, selectedGenre, BUF); // citeste de la client genul ales (2)
					topByGenre(songs, selectedGenre);
					write(client, songs, BUF); // trimite la client genres (3)
				}

				else if (strcmp(input, "topByVotes") == 0)
				{
					char songs[BUF];
					topByVotes(songs);
					printf("Songs:\n \n %s\n \n", songs);
					write(client, songs, BUF); // trimite la client genres
				}

				else if (strcmp(input, "showUsers") == 0)
				{
					char users[BUF];
					showUsers(users);
					printf("Users: %s\n \n", users);
					write(client, users, BUF); // trimite la client users
				}

				else if (strcmp(input, "setVoteRight") == 0)
				{
					int userID, canVote;
					char canVote_as_string[BUF];
					char ID_as_string[BUF];
					char users[BUF];
					showUsers(users);
					printf("Users: %s\n \n", users);
					write(client, users, BUF);		 // trimite la client users (1)
					read(client, ID_as_string, BUF); // citeste id user de la client (2)
					userID = atoi(ID_as_string);
					read(client, canVote_as_string, BUF); // citeste 1 sau 0 pt voterights (3)
					canVote = atoi(canVote_as_string);
					setVoteRight(userID, canVote);
				}

				else if (strcmp(input, "vote") == 0)
				{
					char canVote[BUF];
					sprintf(canVote, "%d", getVoteRight(ID));
					write(client, canVote, BUF); // scrie la client validare drept de vot (1)
					if (voteFlag == 1 || adminFlag == 1)
					{
						int songID;
						char ID_as_string[BUF];
						char songs[BUF];
						showSongs(songs);
						write(client, songs, BUF);		 // trimite la client lista de cantece (2)
						read(client, ID_as_string, BUF); // citeste de la client songID (3)
						songID = atoi(ID_as_string);
						voteSong(songID);
						printf("Song with ID = %d was successfully voted.", songID);
					}
				}

				else
				{
					printf("The following command failed: %s\n", output);
					bzero(output, BUF);
					strcpy(output, "Error! Unknown command.\n");
					bzero(input, BUF);
				}
			}
		}
	}
}