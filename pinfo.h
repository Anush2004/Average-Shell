#ifndef SPECIFICATION_5
#define SPECIFICATION_5

extern int no_of_background ;
extern int pid_array[1000];
extern char bg_command[1000][100];
extern int time_flag;
extern int tempindex ;
extern int traversor;
extern int inputfd, outputfd, stdinClone, stdoutClone;

// process info using pid
int pinfo(char *homedir, char **strings);

#endif
