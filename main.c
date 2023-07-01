#include "headers.h"
#include "cd.h"
#include "ls.h"
#include "fgbg.h"
#include "pinfo.h"
#include "discover.h"
#include "history.h"

int no_of_background = 0;
int pid_array[1000];
char bg_command[1000][100];
int time_flag = 0;
int tempindex = 0;
int traversor = 0;
char temp[100][100];
int inputfd = 0, outputfd = 0, stdinClone, stdoutClone;
char bg_input[1000][1000];
int order[1000];
int fg_proc = 0;
char fg_com[1000];
pid_t ret =0;

// Shell initializing
void initializeshell()
{

    printf("\e[1;1H\e[2J"); // clear screen
    printf("\nHelloo There!!\n");
    sleep(1);               // waits for 1 second;
    printf("\e[1;1H\e[2J"); // clear  screen
}

// Directory printing function
void printDir(char *currdir, char *homedir)
{
    int f;
    char username[50];
    char Sysname[50];
    f = gethostname(Sysname, sizeof(Sysname));
    if (f == -1)
    {
        perror("systemname: ");
    }

    f = getlogin_r(username, sizeof(username));
    if (f == -1)
    {
        perror("username: ");
    }

    // if in shell directory replace it with ~
    if (strstr(currdir, homedir))
    {
        char cwd[1024];
        cwd[0] = '~';
        int i = strlen(homedir) - 1, j = 1;
        for (i = strlen(homedir); currdir[i] != '\0'; i++)
        {
            cwd[j] = currdir[i];
            j++;
        }
        cwd[j] = '\0';
        if (time_flag > 1)
        {
            printf("%s<%s@%s:%s%stook %ds%s>", KGRN, username, Sysname, KBLU, cwd, time_flag, KNRM);
            time_flag = 0;
        }
        else
            printf("%s<%s@%s:%s%s%s>", KGRN, username, Sysname, KBLU, cwd, KNRM);
    }
    else
    {
        if (time_flag > 1)
        {
            printf("%s<%s@%s:%s%stook %ds%s>", KGRN, username, Sysname, KBLU, currdir, time_flag, KNRM);
            time_flag = 0;
        }
        else
            printf("%s<%s@%s:%s%s%s>", KGRN, username, Sysname, KBLU, currdir, KNRM);
    }
}

// background helper function
void bg_handle()
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0)
    {
        int i = 0;
        while (pid_array[i] != pid)
        {
            i++;
            if (i == 1000)
            {
                return;
            }
        }

        if (WIFSTOPPED(status))
            return;

        if (WIFEXITED(status))
        {
            sprintf(temp[tempindex++], "%s with PID %d exited %s\n", bg_command[i], pid_array[i], "normally");
        }
        else
        {
            sprintf(temp[tempindex++], "%s with PID %d exited %s\n", bg_command[i], pid_array[i], "abnormally");
        }
    }
}

// for separating input to command and strings
void parseline(char *input, char *command, char **strings)
{
    char *part_with_tab_or_line;
    char *parts[100];
    int i = 0;
    char temp2[1024];
    strcpy(temp2, input);
    int input_output = -1;
    part_with_tab_or_line = strtok(temp2, " \t \n ");
    while (part_with_tab_or_line != NULL)
    {
        if (input_output != -1)
        {
            if (input_output == 1)
            {
                outputfd = open(part_with_tab_or_line, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                input_output = -1;
            }
            else if (input_output == 2)
            {
                outputfd = open(part_with_tab_or_line, O_WRONLY | O_CREAT | O_APPEND, 0644);
                input_output = -1;
            }
            else if (input_output == 0)
            {
                inputfd = open(part_with_tab_or_line, O_RDONLY);
                input_output = -1;
            }
        }
        parts[i] = strdup(part_with_tab_or_line);
        if (!strcmp(parts[i], ">"))
        {
            input_output = 1;
        }
        else if (!strcmp(parts[i], ">>"))
        {
            input_output = 2;
        }
        else if (!strcmp(parts[i], "<"))
        {
            input_output = 0;
        }
        i++;
        part_with_tab_or_line = strtok(NULL, " \t \n");
    }
    if (inputfd < 0 || outputfd < 0)
    {
        perror("Opening file");
    }

    if (inputfd)
    {
        stdinClone = dup(0);
        dup2(inputfd, 0);
    }
    if (outputfd)
    {
        stdoutClone = dup(1);
        dup2(outputfd, 1);
    }

    // first word is command and remaining are parameters
    int j = 0;
    while (j < i - 1)
    {
        strings[j] = parts[j + 1];
        j++;
    }
    strings[j] = NULL;
    strcpy(command, parts[0]);
}

// Shell Exiting
void exitshell()
{
    printf("\e[1;1H\e[2J");
    printf("\nThank You for using this shell\n");
    printf("\nWill try to implement more features soon, Wish me luck!:)\n");
    sleep(1); // waits for 1 second;
    printf("\e[1;1H\e[2J");
    exit(0);
}

// tab autocomplete
void do_tab(char *input, int *index, char *homedir)
{
    int in = strlen(input) - 1;
    while (in >= 0)
    {
        if (input[in] == ' ')
        {
            break;
        }
        in--;
    }
    char temp[1000];

    strcpy(temp, input + in + 1);

    char path[1000];
    strcpy(path, temp);
    char search[1000];
    int i = 0;
    while (i < strlen(temp))
    {
        if (temp[i] == '/')
        {
            strncpy(path, temp, i + 1); // abcd/efgh abcd/ is path efgh is search from which we need to search
            path[i + 1] = '\0';
            strcpy(search, temp + i + 1);
        }
        i++;
    }
    if (path[strlen(path) - 1] != '/')
    {
        strcpy(search, path);
        strcpy(path, "");
    }
    // path details
    if (path[0] == '~')
    {
        if (path[1] == '/')
        {
            sprintf(path, "%s%s", homedir, path + 1);
        }
        else if (path[1] == '\0')
        {
            sprintf(path, "%s", homedir);
        }
    }
    else
    {
        if (path[0] != '/') // if first is / then its absolute path
        {
            char cwd[500];
            getcwd(cwd, sizeof(cwd));
            char temp[500];
            strcpy(temp, path);
            sprintf(path, "%s/%s", cwd, temp);
        }
    }

    struct dirent *d;
    DIR *dir = opendir(path);

    if (dir != NULL)
    {
        d = readdir(dir);
        char m[1000];
        int m_count = 0;
        while (d != NULL)
        {
            if (strlen(search) <= strlen(d->d_name) && strncmp(search, d->d_name, strlen(search)) == 0)
            {
                m_count++;
                strcpy(m, d->d_name);
            }
            d = readdir(dir);
        }
        closedir(dir);
        if (m_count == 1) // for m_count = 0 we dont change anything
        {
            char name[2000];
            sprintf(name, "%s%s", path, m);
            if (checkiffile(name) == 1)
            {
                for (int i = strlen(search); i < strlen(m); i++)
                {
                    printf("%c", m[i]);
                    input[*index] = m[i];
                    *index += 1;
                }
                printf(" ");
                input[*index] = ' ';
                *index += 1;
            }
            else
            {
                for (int i = strlen(search); i < strlen(m); i++)
                {
                    printf("%c", m[i]);
                    input[*index] = m[i];
                    *index += 1;
                }
                printf("/");
                input[*index] = '/';
                *index += 1;
            }
        }
        else if (m_count > 1)
        {
            char pre[1000] = "";
            int i_count = 0;
            dir = opendir(path);

            d = readdir(dir);
            while (d != NULL)
            {
                if (strncmp(search, d->d_name, strlen(search)) == 0 && strlen(search) <= strlen(d->d_name))
                {
                    if (i_count == 0)
                    {
                        strcpy(pre, d->d_name);
                        i_count++;
                        printf("\n%s", d->d_name);
                    }
                    else
                    {
                        char t_pre[1000];
                        for (int i = 0; i < strlen(pre); i++)
                        {
                            if (pre[i] != d->d_name[i])
                            {
                                strncpy(t_pre, pre, i);
                                t_pre[i] = '\0';
                                break;
                            }
                        }
                        strcpy(pre, t_pre); // stores most common
                        printf("\n%s", d->d_name);
                    }
                }
                d = readdir(dir);
            }

            for (int i = strlen(search); i < strlen(pre); i++)
            {
                input[*index] = pre[i];
                *index += 1;
            }
            input[*index] = '\0';
            printf("\n");
            char cwd[1000];
            getcwd(cwd, sizeof(cwd));
            printDir(cwd, homedir);
            printf("%s", input);
        }
    }
}

// for sorting background
void sort_background()
{
    char temp[1000];
    int temp_1, temp_2;

    for (int i = 0; i < no_of_background; i++)
    {
        for (int j = i + 1; j < no_of_background; j++)
        {
            if (strcmp(bg_input[j - 1], bg_input[j]) > 0)
            {
                //
                strcpy(temp, bg_input[j - 1]);
                strcpy(bg_input[j - 1], bg_input[j]);
                strcpy(bg_input[j], temp);
                //
                temp_1 = order[j - 1];
                order[j - 1] = order[j];
                order[j] = temp_1;
                //
                temp_2 = pid_array[j - 1];
                pid_array[j - 1] = pid_array[j];
                pid_array[j] = temp_2;
                //
            }
        }
    }
}

// ctrlz function for signal
void ctrlz(int signum)
{
    if (fg_proc != 0)
    {
        if (kill(ret, SIGTSTP) < 0)
        {
            perror("Unable to get fg process to bg");
        }
        pid_array[no_of_background] = ret;
        strcpy(bg_input[no_of_background], fg_com);
        order[no_of_background] = no_of_background;
        ++no_of_background;
    }
    else
    {
        fg_proc=0;
        return;
    }
}

// kill fg function for signal
void kilfg(int signum)
{
    if (fg_proc == 0)
    {
        fg_proc=0;
        return;
    }
    else
    {
        printf("\n");
    }
}

// run jobs command
void jobs(char **strings)
{
    int r_flag = 0, s_flag = 0;
    if (strings[0] == NULL)
    {
        r_flag = 1;
        s_flag = 1;
    }
    for (int i = 0; strings[i] != NULL; i++)
    {
        if (strstr(strings[i], "r"))
        {
            r_flag = 1;
        }
        if (strstr(strings[i], "s"))
        {
            s_flag = 1;
        }
    }
    sort_background();
    for (int i = 0; i < no_of_background; i++)
    {
        char path[1000];
        sprintf(path, "/proc/%d/stat", pid_array[i]);

        FILE *process_file = fopen(path, "r");
        if (process_file != NULL)
        {
            char fileStat[1000];
            fread(fileStat, 1000, 1, process_file);
            fclose(process_file);

            // A pointer for each space seperated attribute in the stat file
            char *attrs[1001] = {NULL};
            attrs[0] = strtok(fileStat, " ");
            for (int j = 1; attrs[j - 1] != NULL; j++)
                attrs[j] = strtok(NULL, " ");

            char status[100];
            if (strcmp(attrs[2], "S") == 0 || strcmp(attrs[2], "R") == 0)
            {
                strcpy(status, "Running");
            }
            else
            {
                strcpy(status, "Stopped");
            }
            if (r_flag == 1 && s_flag == 1)
            {
                printf("[%d] %s %s [%d]\n", order[i], status, bg_input[i], pid_array[i]);
            }
            else if (r_flag == 1)
            {
                if (strcmp(status, "Running") == 0)
                {
                    printf("[%d] %s %s [%d]\n", order[i], status, bg_input[i], pid_array[i]);
                }
            }
            else if (s_flag == 1)
            {
                if (strcmp(status, "Stopped") == 0)
                {
                    printf("[%d] %s %s [%d]\n", order[i], status, bg_input[i], pid_array[i]);
                }
            }
        }
    }
}

// run sig command
void sig(char **strings)
{
    sort_background();
    int j_index = atoi(strings[0]);
    int signal = atoi(strings[1]);

    if (j_index > no_of_background)
    {
        perror("index exceeded");
        return;
    }
    int index = -1;
    int i = 0;
    for (int j = 0; j < no_of_background; j++)
    {
        if (order[j] == j_index)
        {
            j_index = j;
        }
    }
    while (1)
    {
        char path[100];
        sprintf(path, "/proc/%d/stat", pid_array[i]);
        FILE *process_file;
        process_file = fopen(path, "r");
        if (process_file != NULL)
        {
            ++index;
        }
        if (j_index == index)
        {
            kill(pid_array[index], signal);
            break;
        }
        i++;
    }
}

// run bg command
void bacgro(char **strings)
{
    sort_background();
    int j_index = atoi(strings[0]);

    if (j_index > no_of_background)
    {
        perror("index exceeded");
        return;
    }
    int index = -1;
    int i = 0;
    for (int j = 0; j < no_of_background; j++)
    {
        if (order[j] == j_index)
        {
            j_index = j;
        }
    }
    while (1)
    {
        char path[100];
        sprintf(path, "/proc/%d/stat", pid_array[i]);
        FILE *process_file;
        process_file = fopen(path, "r");
        if (process_file != NULL)
        {
            ++index;
        }
        if (j_index == index)
        {
            kill(pid_array[index], 18);
            break;
        }
        i++;
    }
}

// run fg command
void forgro(char **strings)
{
    sort_background();
    int j_index = atoi(strings[0]);

    if (j_index > no_of_background)
    {
        perror("index exceeded");
        return;
    }
    int index = -1;
    int i = 0;
    for (int j = 0; j < no_of_background; j++)
    {
        if (order[j] == j_index)
        {
            j_index = j;
        }
    }
    pid_t pid;
    while (1)
    {
        char path[100];
        sprintf(path, "/proc/%d/stat", pid_array[i]);
        FILE *process_file;
        process_file = fopen(path, "r");
        if (process_file != NULL)
        {
            ++index;
        }
        if (j_index == index)
        {
            pid = pid_array[index];
            pid_array[index] = -1;
            break;
        }
        i++;
    }

    setpgid(pid, getpgid(0));

    //
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, pid);
    //
    //
    if (!(kill(pid, SIGCONT) >= 0))
    {
        perror("cannot run bg process in fg");
    }
    int status;
    waitpid(pid, &status, WUNTRACED);
    //
    //
    tcsetpgrp(0, getpgid(0));
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    //
}

// used to check if its file or something else
int checkiffile(const char *file)
{
    struct stat path_stat;
    stat(file, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

// executes commands by calling parsing and proper commands
void execCommand(char *part_, char *command, char **strings, char *homedir, char *pastpath)
{
    if (strstr(part_, "&"))
    {
        readinput_and(part_);
    }
    else
    {
        // parse input into command and other strings
        parseline(part_, command, strings);

        // check command and apply it
        commander(part_, homedir, pastpath, command, strings);
    }
}

// pipe handler function
void handlepipes(int pip_count, char *part_, char *command, char **strings, char *homedir, char *pastpath)
{
    char *com[pip_count + 1];
    char *rest1 = part_;
    int comcount = 0;
    while (1)
    {
        com[comcount] = strtok_r(rest1, "|", &rest1);
        if (com[comcount] == NULL)
        {
            break;
        }
        comcount++;
    }
    for (int k = 0; k < comcount; k++)
    {
        if (com[k][0] == ' ')
        {
            com[k]++;
        }
        if (com[k][strlen(com[k]) - 1] == ' ')
        {
            com[k][strlen(com[k]) - 1] = '\0';
        }
    }

    int fds[pip_count][2];

    for (int i = 0; i < pip_count; i += 1)
    {
        if (pipe(fds[i]))
        {
            perror("pipe:");
            return;
        }
    }
    int pid;
    int exitCode = 1;
    for (int k = 0; k < comcount; k++)
    {
        pid = fork();
        if (pid == 0)
        {
            if (k < pip_count)
                dup2(fds[k][1], 1);
            if (k > 0)
            {
                close(fds[k - 1][1]);
                dup2(fds[k - 1][0], 0);
            }
            char *par = com[k];
            execCommand(par, command, strings, homedir, pastpath);
            exit(exitCode);
        }
    }
    for (int l = 0; l < pip_count; l++)
    {
        close(fds[l][0]);
        close(fds[l][1]);
    }
    for (int l = 0; l < pip_count; l++)
    {
        if (l == comcount - 1)
        {
            wait(&exitCode);
            exitCode = WEXITSTATUS(exitCode);
        }
        else
            wait(NULL);
    }
}

// String input
int takeinput(char *input, char *homedir, char *pastpath)
{
    char buff;
    int index = 0;
    char command[100];
    char *strings[1000];

    struct termios o1, o2;
    tcgetattr(STDIN_FILENO, &o1);
    o2 = o1;
    o2.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &o2);
    setbuf(stdout, NULL);

    char in;
    while (read(STDIN_FILENO, &in, 1) == 1)
    {
        if (!iscntrl(in))
        {
            printf("%c", in);
            input[index++] = in;
            input[index] = '\0';
        }
        else
        {
            if (in == 10) // new line character
            {
                printf("\n"); // print new line in stdin
                break;
            }
            else if (in == 9) // input has tab
            {
                do_tab(input, &index, homedir);
            }
            else if (in == 127) // input is backspace
            {
                if (index > 0)
                {
                    input[--index] = '\0';
                    printf("\b \b");
                }
            }
            else if (in == 4)
            {
                printf("\n");
                exitshell();
                exit(0);
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &o1);

    if (index == 0)
    {
        // line empty then return 0 to just continue
        return 0;
    }
    char *rest = input;
    char *part_[100];
    int i = 0;
    // ; separation
    while (1)
    {
        part_[i] = strtok_r(rest, ";", &rest);
        if (part_[i] == NULL)
        {
            break;
        }
        i++;
    }

    // pipe checking
    for (int j = 0; j < i; j++)
    {
        int pip_count = 0;
        int len = strlen(part_[j]);
        for (int k = 0; k < len; k++)
        {
            if (part_[j][k] == '|')
            {
                pip_count++;
            }
        }
        if (pip_count == 0)
            execCommand(part_[j], command, strings, homedir, pastpath);
        else
        {
            handlepipes(pip_count, part_[j], command, strings, homedir, pastpath);
        }
    }
}

// to execute commands other than cd ls or commands required using execvp
void fg1(char *command, char **strings)
{
    time_t start;
    start = time(NULL);
    char *string1[1000];
    int i = 0;
    string1[0] = command;
    fg_com[0] = '\0';
    strcat(fg_com, command);
    
    for (int i = 0; strings[i] != NULL; i++)
    {
        strcat(fg_com, " ");
        strcat(fg_com, strings[i]);
    }
    while (strings[i] != NULL)
    {
        string1[i + 1] = strings[i];
        i++;
    }
    string1[i + 1] = NULL;
    ret = fork();
    int status;
    fg_proc = 1;

    if (ret != 0)
    {
        waitpid(ret, &status, 0);
        time_t end;
        end = time(NULL);
        time_flag = end - start;
        fg_proc = 0;
    }
    else
    {
        if (execvp(string1[0], string1) < 0)
        {
            printf("Error: Command not found : %s\n", string1[0]);
        }
    }
}

// searches the command to call the function
void commander(char *input, char *homedir, char *pastpath, char *command, char **strings)
{
    for (int i = 0; strings[i] != NULL; i++)
    {
        if (!strcmp(strings[i], ">"))
        {
            strings[i] = NULL;
            continue;
        }
        if (!strcmp(strings[i], "<"))
        {
            strings[i] = NULL;
            continue;
        }
        if (!strcmp(strings[i], ">>"))
        {
            strings[i] = NULL;
            continue;
        }
    }
    if (strcmp(command, "clear") == 0)
    {
        clrfunc();
    }
    else if (strcmp(command, "cd") == 0)
    {
        cdfunc(homedir, pastpath, strings);
    }
    else if (strcmp(command, "pwd") == 0)
    {
        pwdfunc();
    }
    else if (strcmp(command, "exit") == 0)
    {
        exitshell();
    }
    else if (strcmp(command, "echo") == 0)
    {
        echfunc(strings);
    }
    else if (strcmp(command, "ls") == 0)
    {
        lsfunc(homedir, strings);
    }
    else if (strcmp(command, "pinfo") == 0)
    {
        pinfo(homedir, strings);
    }
    else if (strcmp(command, "history") == 0)
    {
        int n = 0;
        while (strings[n] != NULL)
        {
            n++;
        }
        if (n == 0)
        {
            show_history(10);
        }
        else
        {
            if (atoi(strings[0]) <= 0 || atoi(strings[0]) > 20)
            {
                printf("history <int n> \n n > 0 && n <= 20\n");
                return;
            }
            show_history(atoi(strings[0]));
        }
    }
    else if (strcmp(command, "discover") == 0)
    {
        discoverfunc(strings, homedir);
    }
    else if (strcmp(command, "jobs") == 0)
    {
        jobs(strings);
    }
    else if (strcmp(command, "sig") == 0)
    {
        sig(strings);
    }
    else if (strcmp(command, "bg") == 0)
    {
        bacgro(strings);
    }
    else if (strcmp(command, "fg") == 0)
    {
        forgro(strings);
    }
    else
    {
        fg1(command, strings);
    }
    if (inputfd)
    {
        close(inputfd);
        dup2(stdinClone, 0);
        close(stdinClone);
    }
    if (outputfd)
    {
        close(outputfd);
        dup2(stdoutClone, 1);
        close(stdoutClone);
    }
}

int main(int argc, char **argv)
{
    initializeshell();
    char input[1024];
    int f;
    char *f1;
    char pastpath[100];
    f1 = getcwd(pastpath, sizeof(pastpath));

    if (f1 == NULL)
    {
        perror("cwd: ");
    }
    char homedir[100];
    long long int count = 0, count1 = 0;
    f1 = getcwd(homedir, sizeof(homedir));
    if (f1 == NULL)
    {
        perror("cwd: ");
    }
    signal(SIGINT, kilfg);
    signal(SIGTSTP, ctrlz);
    signal(SIGCHLD, bg_handle);
    // infinite loop so the shell can run
    while (1)
    {
        char currentdir[100];
        f1 = getcwd(currentdir, sizeof(currentdir));
        if (f1 == NULL)
        {
            perror("cwd: ");
        }

        for (int i = traversor; temp[i][0] != '\0'; ++i)
        {
            printf("%s", temp[traversor++]);
        }
        // print line about name and system name also directory
        printDir(currentdir, homedir);
        // take input
        takeinput(input, homedir, pastpath);
    }
}