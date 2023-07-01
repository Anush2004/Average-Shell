#include "headers.h"
#include "discover.h"
#include "ls.h"

// used to find all files used discover
void find_all_files(char *path, int od, int of)
{
    struct dirent **temp;
    int no_of_files = scandir(path, &temp, 0, alphasort_case_insensitive);
    int i = 0;
    while (i < no_of_files)
    {
        char *names = temp[i]->d_name;
        char new_pathway[1024] = {'\0'};
        strcpy(new_pathway, path);
        strcat(new_pathway, "/");
        strcat(new_pathway, names);

        if (checkiffile(new_pathway) != 1 && names[0] != '.')
        {
            if (od == 1)
            {
                printf("%s\n", new_pathway);
            }
            find_all_files(new_pathway, od, of);
        }
        else if (checkiffile(names) == 1 && of == 1 && names[0] != '.')
        {
            printf("%s\n", new_pathway);
        }
        i++;
    }
}

// used to search a particular file
void search_all_files(char *path, int od, int of, char **file)
{
    struct dirent **temp;
    int no_of_files = scandir(path, &temp, 0, alphasort_case_insensitive);
    int i = 0;

    while (i < no_of_files)
    {
        char *names = temp[i]->d_name;
        char new_pathway[1024] = {'\0'};
        char file_pathway[1024] = {'\0'};
        strcpy(new_pathway, path);
        strcat(new_pathway, "/");
        strcat(new_pathway, names);
        if (strstr(new_pathway, "./") == NULL)
        {
            strcpy(file_pathway, "./");
            strcat(file_pathway, new_pathway);
        }
        else
        {
            strcpy(file_pathway, new_pathway);
        }
        if (checkiffile(file_pathway) != 1 && names[0] != '.')
        {
            search_all_files(new_pathway, od, of, file);
            if (od == 1 && strcmp(file[0], names) == 0)
            {
                printf("%s\n", new_pathway);
            }
        }
        else if (checkiffile(file_pathway) == 1 && of == 1)
        {
            if (strcmp(new_pathway, ".") != 0 && names[0] != '.' && strcmp(file[0], names) == 0)
            {
                printf("%s\n", new_pathway);
            }
        }
        i++;
    }
}

// used for discover command, sets flags,file ,target directory from stdin to further functions
void discoverfunc(char **strings, char *homedir)
{
    char **flag;
    char **file;
    char **temp;
    char dir[100] = ".";
    flag = (char **)malloc(sizeof(char *) * 10);
    for (int j = 0; j < 10; j++)
    {
        flag[j] = (char *)malloc(sizeof(char) * 120);
    }

    file = (char **)malloc(sizeof(char *) * 20);
    temp = (char **)malloc(sizeof(char *) * 20);
    for (int j = 0; j < 20; j++)
    {
        temp[j] = (char *)malloc(sizeof(char) * 120);
        file[j] = (char *)malloc(sizeof(char) * 120);
        strcpy(file[j], "");
        strcpy(temp[j], "");
    }
    int flaga = 0;
    int arga = 0;
    int index = 0;

    for (int index = 0; strings[index] != NULL; index++)
    {
        if (strstr(strings[index], "-"))
        {
            strcpy(flag[flaga++], strings[index]);
        }
        else if (strstr(strings[index], "\""))
        {
            strcpy(temp[arga], strings[index]);
            int i;
            if (strstr(temp[arga], "/"))
            {
                for (i = strlen(temp[arga]) - 2; temp[i] != "/"; i--)
                {
                    file[arga][i - 1] = temp[arga][i];
                }
            }
            else
            {
                for (i = 1; temp[arga][i] != '\"'; i++)
                {
                    file[arga][i - 1] = temp[arga][i];
                }
            }

            file[arga][i - 1] = '\0';
            arga++;
        }
        else
        {
            strcpy(dir, strings[index]);
            if (strcmp(dir, "~") == 0)
            {
                strcpy(dir, homedir);
            }
        }
    }
    flag[flaga] = NULL;
    file[arga] = NULL;
    int od = 0, of = 0;
    for (int j = 0; j < flaga; j++)
    {
        if (strstr(flag[j], "-d"))
        {
            od += 1;
        }
        if (strstr(flag[j], "-f"))
        {
            of += 1;
        }
    }
    if (flaga == 0)
    {
        od = 1;
        of = 1;
    }
    if (arga == 0)
    {
        if (od == 1)
        {
            printf("%s\n", dir);
        }
        find_all_files(dir, od, of);
    }
    else
    {
        search_all_files(dir, od, of, file);
    }
}
