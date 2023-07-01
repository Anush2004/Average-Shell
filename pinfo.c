#include "headers.h"
#include "pinfo.h"


// process info using pid
int pinfo(char *homedir, char **strings)
{
    int pid = 0;
    if (strings[0] != NULL)
    {
        pid = (int)atoi(strings[0]);
    }
    if (pid == 0)
    {
        pid = getpid();
    }

    char path_process_file[50];
    sprintf(path_process_file, "/proc/%d/stat", (int)pid);

    char fileStat[1000] = {'\0'};
    FILE *procStat;
    procStat = fopen(path_process_file, "r");
    if (procStat == NULL)
    {
        perror("process status: ");
        return 0;
    }
    fread(fileStat, 1000, 1, procStat);
    fclose(procStat);

    // A pointer for each space seperated attribute in the stat file
    char *attrs[1001] = {NULL};
    attrs[0] = strtok(fileStat, " ");
    for (int i = 1; attrs[i - 1] != NULL; i++)
        attrs[i] = strtok(NULL, " ");

    // Print required information from stat file
    int par = 0;
    printf("pid : %d\n", (int)pid);
    if (strcmp(attrs[4], attrs[7]) == 0)
    {
        strcat(attrs[2], "+");
    }
    printf("process status : %s\n", attrs[2]);

    printf("memory : %s\n", attrs[22]);
    // Read the exe link to get executable path
    sprintf(path_process_file, "/proc/%d/exe", (int)pid);
    char execPath[4097] = {'\0'};
    memset(execPath, 0, 4097);
    int readStat = readlink(path_process_file, execPath, 4097);
    if (strstr(execPath, homedir))
    {
        char cwd[1024];
        cwd[0] = '~';
        int i = strlen(homedir) - 1, j = 1;
        for (i = strlen(homedir); execPath[i] != '\0'; i++)
        {
            cwd[j] = execPath[i];
            j++;
        }
        cwd[j] = '\0';
        if (readStat == -1)
            perror("readlink: ");
        printf("executable path : %s\n", readStat == -1 ? "Doesn't exist" : cwd);
    }
    else
    {
        // If it is a zombie process, exe doesn't exist, prompt
        if (readStat == -1)
            perror("readlink: ");
        printf("executable path : %s\n", readStat == -1 ? "Doesn't exist" : execPath);
    }
    return 0;
}