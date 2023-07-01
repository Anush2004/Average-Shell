#ifndef SPECIFICATION_2
#define SPECIFICATION_2

extern int no_of_background ;
extern int pid_array[1000];
extern char bg_command[1000][100];
extern int time_flag;
extern int tempindex ;
extern int traversor;
extern int inputfd, outputfd, stdinClone, stdoutClone;

// cd function
void cdfunc(char *homedir, char *pastpath, char **strings);

// echo function
void echfunc(char **strings);

// pwd function
void pwdfunc();

// Clear Function
void clrfunc();

// sleep function
void sleepfunc(char **strings);

#endif