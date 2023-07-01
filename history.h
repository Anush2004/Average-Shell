#ifndef SPECIFICATION_8
#define SPECIFICATION_8

extern int no_of_background ;
extern int pid_array[1000];
extern char bg_command[1000][100];
extern int time_flag;
extern int tempindex ;
extern int traversor;
extern int inputfd, outputfd, stdinClone, stdoutClone;

char temp[100][100];
// max function
int max(int a, int b);

// function to add history
void add_history(char *line);

// function to show ur history
void show_history(int no);

#define history_file "/tmp/.shell_history"
#define size 5000

#endif