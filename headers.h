#ifndef HEADERS_H
#define HEADERS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <ctype.h>

// appropriate coloring wherever required
#define KGRN "\x1B[32m"
#define KNRM "\x1B[0m"
#define KBLU "\x1B[34m"

// used to check if its file or something else
int checkiffile(const char *file);

// Shell initializing
void initializeshell();

// String input
int takeinput(char *input, char *homedir, char *pastpath);

// background helper function
void bg_handle();

// Directory printing function
void printDir(char *currdir, char *homedir);

// for separating input to command and strings
void parseline(char *input, char *command, char **strings);

// searches the command to call the function
void commander(char *input, char *homedir, char *pastpath, char *command, char **strings);

// executes commands other commands which were not written using execvp
void fg1(char *command, char **strings);

// executes commands by calling parsing and proper commands
void execCommand(char *part_, char *command, char **strings, char *homedir, char *pastpath);

// Shell Exiting
void exitshell();

#endif