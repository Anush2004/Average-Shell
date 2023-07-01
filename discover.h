#ifndef SPECIFICATION_7
#define SPECIFICATION_7

extern int no_of_background ;
extern int pid_array[1000];
extern char bg_command[1000][100];
extern int time_flag;
extern int tempindex ;
extern int traversor;
extern int inputfd, outputfd, stdinClone, stdoutClone;

// used to find all files used discover
void find_all_files(char *path, int od, int of);

// used to search a particular file
void search_all_files(char *path, int od, int of, char **file);

// used for discover command, sets flags,file ,target directory from stdin to further functions
void discoverfunc(char **strings, char *homedir);

#endif