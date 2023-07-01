#include "headers.h"
#include "cd.h"

// cd function
void cdfunc(char *homedir, char *pastpath, char **strings)
{
    int f;
    char * f1;
    char currdir[200];
    f1= getcwd(currdir, sizeof(currdir));
    if (f1 == NULL)
    {
        perror("cwd:");
    }
    if(strings[0] != NULL  && strings[1] != NULL)
    {
        printf("Too many arguments!\n");
        return;
    }

    // Empty string after cd => switches to home directory
    if (strings[0] == NULL)
    {
        char username[50];
        f = getlogin_r(username, sizeof(username));
        if (f == -1)
        {
            perror("username");
        }
        char home[100];
        strcpy(home, "/home/");
        strcat(home, username);
        f = chdir(home);
        if (f == -1)
        {
            perror("chdir: ");
        }
        return;
    }
    // cd . -> remains in same directory but the previous directory changes
    else if (strcmp(strings[0], ".") == 0)
    {
        strcpy(pastpath, currdir);
        return;
    }

    // cd.. -> goes to  the directory before it
    else if (strcmp(strings[0], "..") == 0)
    {
        if (strcmp(currdir, "/home") == 0)
        {
            f = chdir("/");
            if (f == -1)
            {
                perror("chdir: ");
            }
            strcpy(pastpath, currdir);
            return;
        }
        else
        {
            char newdir[200];
            int i = strlen(currdir);
            while (i >= 0 && currdir[i] != '/')
            {
                i--;
            }
            strncpy(newdir, currdir, i);
            newdir[i] = '\0';
            f = chdir(newdir);
            if (f == -1)
            {
                perror("chdir: ");
            }
            strcpy(pastpath, currdir);
            return;
        }
    }
    // cd- => switches and prints previous directory
    else if (strcmp(strings[0], "-") == 0)
    {
        f = chdir(pastpath);
        if (f == -1)
        {
            perror("chdir: ");
        }
        printf("%s\n", pastpath);
        strcpy(pastpath, currdir);
        return;
    }
    // if the path is just ~
    else if (strcmp(strings[0], "~") == 0)
    {
        f = chdir(homedir);
        if (f == -1)
        {
            perror("chdir: ");
        }
        strcpy(pastpath, currdir);
        return;
    }
    // if the path is ~/something
    else if (strings[0][0] == '~' && strings[0][1] != '\0')
    {
        char homedir2[100];
        strcpy(homedir2, homedir);
        strncat(homedir2, strings[0] + 1, strlen(strings[0]) - 1);
        DIR *directory = opendir(homedir2);
        if (directory == NULL)
        {
            perror("r1");
        }
        else
        {
            chdir(homedir2);
            strcpy(pastpath, currdir);
        }
    }
    else
    {
        DIR *directory = opendir(strings[0]);
        if (directory == NULL)
        {
            perror("directory: ");
        }
        else
        {
            f = chdir(strings[0]);
            if (f == -1)
            {
                perror("chdir: ");
            }
            strcpy(pastpath, currdir);
        }
    }
}

// echo function
void echfunc(char **strings)
{
    int i = 0;
    while (strings[i] != NULL)
    {
        if (strcmp(strings[i], "&") == 0 || strcmp(strings[i], ";") == 0 || strcmp(strings[i], ">") == 0 || strcmp(strings[i], "<") == 0  || strcmp(strings[i], ">>") == 0 )
            break;
        printf("%s ", strings[i]);
        i++;
    }
    printf("\n");
}

// pwd function
void pwdfunc()
{
    char currdir[1000];
    char * f;
    f = getcwd(currdir, sizeof(currdir));
    if (f == NULL)
    {
        perror("cwd: ");
        return;
    }
    printf("%s\n", currdir);
}

// sleep function
void sleepfunc(char **strings)
{
    sleep(atoi(strings[0]));
}

// Clear Function
void clrfunc()
{
    printf("\e[1;1H\e[2J");
}
