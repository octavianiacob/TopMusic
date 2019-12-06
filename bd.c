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

char username[100], password[100];
// conexiune baza de date
sqlite3 *database;
sqlite3_stmt *statement;
char *sql = NULL;
char *err_msg = 0;
char str[1000];
int rc;

static int callback(void *str, int argc, char **argv, char **azColName)
{
    int i;
    char *data = (char *)str;

    for (i = 0; i < argc; i++)
    {
        strcat(data, azColName[i]);
        strcat(data, " = ");
        if (argv[i])
            strcat(data, argv[i]);
        else
            strcat(data, "NULL");
        strcat(data, "\n");
    }

    strcat(data, "\n");
    return 0;
}

int main()
{
    rc = sqlite3_open("topmusic.db", &database);
    if (rc)
        printf("Error opening database \n");
    else
        printf("Database opened successfully \n");

    asprintf(&sql, "insert into user (user_name, user_password) values (\"%s\", \"%s\");", "Tom", "Tompassword"); /* 1 */

    printf("%s", sql);
    rc = sqlite3_exec(database, sql, callback, str, &err_msg);
    if (rc != SQLITE_OK)
    {

        printf("SQL error: %s\n", sqlite3_errmsg(database));

        sqlite3_free(err_msg);
    }
    		sqlite3_close(database);
}