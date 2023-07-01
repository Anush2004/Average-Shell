#include "headers.h"
#include "fgbg.h"

// helps and foreground process
void fg(char *command)
{
    strcpy(fg_com,command);
    char *command_words[1000];
    char *token;
    char *rest = command;
    int count_commands = 0;

    time_t start;
    start = time(NULL);
    int input_output = -1;
    while (1)
    {
        token = strtok_r(rest, "\t \n", &rest);

        if (token == NULL)
        {
            break;
        }
        command_words[count_commands] = token;
        ++count_commands;
    }
    command_words[count_commands] = NULL;

    ret = fork();
    int status;
    fg_proc = 1;

    if (ret != 0)
    {
        waitpid(ret, &status, 0);
        fg_proc = 0;
        time_t end;
        end = time(NULL);
        time_flag = end - start;
    }
    else
    {
        if (execvp(command_words[0], command_words) < 0)
        {
            printf("Error: Command not found : %s\n", command_words[0]);
        }
    }
}

// helps and background process
void bg(char *command)
{
    char *command_words[100];
    char *token;
    char *rest = command;
    int count_commands = 0;
    strcpy(bg_input[no_of_background], command);
    while (1)
    {
        token = strtok_r(rest, "\t \n", &rest);
        if (token == NULL)
        {
            break;
        }
        command_words[count_commands] = token;
        ++count_commands;
    }
    command_words[count_commands] = NULL;

    pid_t pid = fork();
    int status;

    if (pid != 0)
    {
        waitpid(pid, &status, WNOHANG);
        order[no_of_background] = no_of_background + 1;
        pid_array[no_of_background] = pid;
        strcpy(bg_command[no_of_background], command_words[0]);
        printf("[%d] %d\n", no_of_background + 1, pid);
        ++no_of_background;
    }
    else
    {
        setpgid(0, 0);
        if (execvp(command_words[0], command_words) < 0)
        {
            printf("Error: Command not found : %s\n", command_words[0]);
        }
    }
}

// reads and parses with & operator
void readinput_and(char *input)
{
    char *token;
    char *rest = input;
    int flag = 0;
    if (input[strlen(input) - 1] == '&')
    {
        flag = 1;
    }
    while (token = strtok_r(rest, "&", &rest))
    {
        if (rest[0] == '\0' && flag == 0)
        {
            fg(token);
        }
        else
        {
            bg(token);
        }
    }
}