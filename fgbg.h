#ifndef SPECIFICATION_4
#define SPECIFICATION_4

extern int no_of_background ;
extern int pid_array[1000];
extern char bg_command[1000][100];
extern int time_flag;
extern int tempindex ;
extern int traversor;
extern int inputfd, outputfd, stdinClone, stdoutClone;
extern char bg_input[1000][1000];
extern int order[1000];
extern int fg_proc;
extern char fg_com[1000];
extern int ret;
// helps and foreground process
void fg(char *command);

// helps and background process
void bg(char *command);

// reads and parses with & operator
void readinput_and(char *input);

#endif