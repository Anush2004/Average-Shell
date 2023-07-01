#ifndef SPECIFICATION_3
#define SPECIFICATION_3
#include <pwd.h>
#include <sys/stat.h>

extern int no_of_background ;
extern int pid_array[1000];
extern char bg_command[1000][100];
extern int time_flag;
extern int tempindex ;
extern int traversor;
extern int inputfd, outputfd, stdinClone, stdoutClone;

// used to sort for dirent in scandir
int alphasort_case_insensitive(const struct dirent **a, const struct dirent **b);

// lsa function can be used to just print all files and directory
// it has additional feature of hidden
void lsafunc(char *dir, int hidden, char *path);

// lsl function can be used to print all files and directory with its info
// it has additional feature of hidden
void lslfunc(char *dir, int hidden, char *path);

// ls function
void lsfunc(char *homedir, char **strings);

#endif