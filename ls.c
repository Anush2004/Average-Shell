#include "headers.h"
#include "ls.h"

// used to sort for dirent in scandir
int alphasort_case_insensitive(const struct dirent **a, const struct dirent **b)
{

    const char *tempa = (char *)malloc(sizeof(char));
    const char *tempb = (char *)malloc(sizeof(char));

    char namea[50], nameb[50];
    tempa = (*a)->d_name;
    tempb = (*b)->d_name;

    int indexa = 0, indexb = 0;
    if (strcmp(tempa, ".") == 0 && strcmp(tempb, "..") == 0 || strcmp(tempb, ".") == 0 && strcmp(tempa, "..") == 0)
    {
        return strcasecmp(tempa, tempb);
    }

    for (int i = 0; i < strlen(tempa); i++)
    {
        if (tempa[i] != '.')
        {
            namea[indexa++] = tempa[i];
        }
        if (tempb[i] != '.')
        {
            nameb[indexb++] = tempb[i];
        }
    }
    namea[indexa] = '\0';
    nameb[indexb] = '\0';
    return strcasecmp(namea, nameb);
}

// lsa function can be used to just print all files and directory
// it has additional feature of hidden
void lsafunc(char *dir, int hidden, char *path)
{
    if (checkiffile(dir))
    {
        return;
    }
    char cwd[1024], initial_dir[1024];
    getcwd(cwd, sizeof(cwd));
    strcpy(initial_dir, cwd);
    struct dirent **temp;
    temp = NULL;

    int no_of_files = scandir(path, &temp, 0, alphasort_case_insensitive);
    chdir(dir);
    if (no_of_files < 0)
    {
        perror("scandir:");
    }
    struct stat file_stat;
    for (long long int i = 0; i < no_of_files; i++)
    {
        char *names = temp[i]->d_name;
        stat(names, &file_stat);
        if ((hidden == 0 && names[0] != '.') || hidden == 1)
        {

            if ((file_stat.st_mode & S_IXOTH) && checkiffile(names) == 1)
            {
                printf("%s", KGRN);
            }
            else
            {
                if (checkiffile(names))
                    printf("%s", KNRM);
                else
                    printf("%s", KBLU);
            }
            printf("%s  ", names);
            printf("%s", KNRM);
            free(temp[i]);
        }
    }
    free(temp);
    chdir(initial_dir);
    printf("\n");
}

// lsa function can be used to print all files and directory with its info
// it has additional feature of hidden
void lslfunc(char *dir, int hidden, char *path)
{
    struct dirent **listtemp;
    int no_of_files = scandir(path, &listtemp, NULL, alphasort_case_insensitive);
    struct stat file_stat;
    struct passwd *pwd;
    char date[12];
    char cwd[100];
    char *g = getcwd(cwd, sizeof(cwd));
    char intial_dir[1024];
    strcpy(intial_dir, cwd);
    long long int temp = 0;
    if (g == NULL)
        perror("cwd: ");
    if (checkiffile(dir) == 1)
    {
        char *names = dir;
        if ((hidden == 0 && names[0] != '.') || hidden == 1)
        {
            stat(names, &file_stat);

            // perms
            printf((checkiffile(names) == 1) ? "-" : "d");
            printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
            printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
            printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
            printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
            printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
            printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
            printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
            printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
            printf((file_stat.st_mode & S_IXOTH) ? "x " : "- ");

            // hard links - number
            long d = file_stat.st_nlink;
            // user name
            struct passwd *pwd;
            // printf("%ld ", file_stat.st_nlink);

            // user name
            // struct passwd *pwd;
            pwd = getpwuid(file_stat.st_uid);
            if (pwd == NULL)
            {
                perror("pwuid user: ");
            }
            printf("%ld %s ", d, pwd->pw_name);

            // group name
            pwd = getpwuid(file_stat.st_gid);
            if (pwd == NULL)
            {
                perror("pwuid group user: ");
            }
            printf("%s ", pwd->pw_name);

            // byte size
            long int k = file_stat.st_size;

            // date and time
            char date[12];
            strftime(date, 20, "%b %2d %2d:%2d ", localtime(&(file_stat.st_mtime)));
            printf("%5ld %s ", k, date);

            // file name
            if ((file_stat.st_mode & S_IXOTH) == 1 && checkiffile(names) == 1)
                printf("%s", KGRN);
            else
            {
                if (checkiffile(names))
                    printf("%s", KNRM);
                else
                    printf("%s", KBLU);
            }
            printf("%s\n%s", names, KNRM);
        }
        return;
    }

    // total
    chdir(dir);
    long long int blocks = 0;
    stat(cwd, &file_stat);
    for (temp = 0; temp < no_of_files; temp++)
    {
        char *names = listtemp[temp]->d_name;
        if (names[0] != '.')
        {
            stat(names, &file_stat);
            blocks += file_stat.st_blocks;
        }
    }
    blocks /= 2;
    printf("total %lld\n", blocks);
    temp = 0;
    while (temp < no_of_files)
    {
        char *names = listtemp[temp]->d_name;
        stat(names, &file_stat);
        if ((hidden == 0 && names[0] != '.') || hidden == 1)
        {
            // perms
            printf((checkiffile(names) == 1) ? "-" : "d");
            printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
            printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
            printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
            printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
            printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
            printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
            printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
            printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
            printf((file_stat.st_mode & S_IXOTH) ? "x " : "- ");

            // hard links - number
            long int k = file_stat.st_nlink;

            // user name
            struct passwd *pwd;
            pwd = getpwuid(file_stat.st_uid);
            if (pwd == NULL)
            {
                perror("pwuid user: ");
            }
            printf("%ld %s ", k, pwd->pw_name);

            // group name
            pwd = getpwuid(file_stat.st_gid);
            if (pwd == NULL)
            {
                perror("pwuid group user: ");
            }
            printf("%s ", pwd->pw_name);

            // byte size
            long int s = file_stat.st_size;

            // date and time
            char date[12];
            strftime(date, 20, "%b %2d %2d:%2d ", localtime(&(file_stat.st_mtime)));
            printf("%5ld %s ", s, date);

            // file name
            if ((file_stat.st_mode & S_IXOTH) == 1 && checkiffile(names) == 1)
                printf("%s", KGRN);
            else
            {
                if (checkiffile(names))
                    printf("%s", KNRM);
                else
                    printf("%s", KBLU);
            }
            printf("%s\n%s", names, KNRM);
        }
        free(listtemp[temp]);
        temp++;
    }
    free(listtemp);
    chdir(intial_dir);
}

// ls function
void lsfunc(char *homedir, char **strings)
{
    char **filelist;
    int no_of_files;
    char *path = ".";
    struct dirent **listtemp;

    //  Just ls shows info of currdir
    if (strings[0] == NULL)
    {
        lsafunc(".", 0, ".");
        return;
    }

    for (int i = 0; strings[i] != NULL; i++)
    {
        //  ls . shows info of currdir
        if (strcmp(strings[i], ".") == 0 && strings[i + 1] == NULL)
        {
            lsafunc(".", 0, ".");
            return;
        }
        //  ls .. shows info of parentdir
        else if (strcmp(strings[i], "..") == 0 && strings[i + 1] == NULL)
        {
            lsafunc("..", 0, "..");
            return;
        }

        // ls -a shows info of hidden files
        else if (strcmp(strings[i], "-a") == 0 && strings[i + 1] == NULL)
        {
            lsafunc(".", 1, ".");
            return;
        }

        // ls ~ shows info of home directory(thats the directory our shell is invoked)
        else if (strcmp(strings[i], "~") == 0 && strings[i + 1] == NULL)
        {
            lsafunc(homedir, 0, homedir);
            return;
        }

        // (ls -l) shows extra info of all files and directories in current directory
        else if (strcmp(strings[i], "-l") == 0 && strings[i + 1] == NULL)
        {
            lslfunc(".", 0, ".");
            return;
        }

        // (ls -al or ls -la ) these prints extra info of all files and dir (even hidden) in current working directory
        else if (((strcmp(strings[i], "-la") == 0) && strings[i + 1] == NULL) || (strcmp(strings[i], "-al") == 0 && strings[i + 1] == NULL))
        {
            lslfunc(".", 1, ".");
            return;
        }
        // ls -a -l or ls -l -a) these prints extra info of all files and dir (even hidden) in current working directory
        else if ((strcmp(strings[i], "-l") == 0 && strcmp(strings[i + 1], "-a") == 0) && strings[i + 2] == NULL || (strcmp(strings[0], "-a") == 0 && strcmp(strings[1], "-l") == 0 && strings[i + 2] == NULL))
        {
            lslfunc(".", 1, ".");
            return;
        }

        // ls <file_name>
        else if (checkiffile(strings[i]) == 1 && strings[i + 1] == NULL)
        {
            if (strstr(strings[i], "/"))
            {
                int j;
                j = strlen(strings[i]) - 1;
                while (j >= 0)
                {
                    if (strings[i][j] == '/')
                    {
                        break;
                    }
                    j--;
                }
                j++;
                char file_name[100];
                int index = 0;
                int k = j;
                while (k < strlen(strings[i]))
                {
                    file_name[index++] = strings[i][k];
                    k++;
                }
                file_name[index] = '\0';
                printf("%s\n", file_name);
            }
            else
            {
                printf("%s\n", strings[i]);
            }
        }

        // ls <directory_name>/ ls <directory_path>
        else if (checkiffile(strings[i]) != 1 && strings[i + 1] == NULL)
        {
            lsafunc(strings[i], 0, strings[i]);
        }

        // ls flags <directory/filename>
        // ls dir_1 -l dir_2 -a file
        else if (strings[i + 1] != NULL)
        {
            char **flag, **arg;
            flag = (char **)malloc(sizeof(char *) * 20);
            arg = (char **)malloc(sizeof(char *) * 20);
            for (int j = 0; j < 10; j++)
            {
                flag[j] = (char *)malloc(sizeof(char) * 120);
                arg[j] = (char *)malloc(sizeof(char) * 120);
            }

            int flaga = 0;
            int arga = 0;
            int index = 0;

            for (int index = 0; strings[index] != NULL; index++)
            {
                if (!(strstr(strings[index], "-")))
                {
                    strcpy(arg[arga++], strings[index]);
                }
                else
                {
                    strcpy(flag[flaga++], strings[index]);
                }
            }
            flag[flaga] = NULL;
            arg[arga] = NULL;

            // sorting of arguments
            for (int i = 0; i < arga; i++)
            {
                for (int j = i + 1; j < arga; j++)
                {
                    char temp[1024];
                    if (strcmp(arg[i], arg[j]) < 0)
                    {
                        strcpy(temp, arg[i]);
                        strcpy(arg[i], arg[j]);
                        strcpy(arg[j], temp);
                    }
                }
            }

            int ol = 0, oa = 0;
            for (int j = 0; j < flaga; j++)
            {
                if (strstr(flag[j], "-a"))
                {
                    ol += 0;
                    oa += 1;
                }
                if (strstr(flag[j], "-l"))
                {
                    ol += 1;
                    oa += 0;
                }
                if (strstr(flag[j], "-la") || strstr(flag[j], "-al"))
                {
                    ol = 1;
                    oa = 1;
                }
            }
            int hidden = 0;
            if (oa >= 1)
            {
                hidden = 1;
            }
            for (int k = arga - 1; k >= 0; k--)
            {
                if (checkiffile(arg[k]) == 1 && ol == 1)
                {
                    lslfunc(arg[k], 0, arg[k]);
                    if (k > 0)
                        printf("\n");
                    continue;
                }
                else if (checkiffile(arg[k]) == 1)
                {
                    printf("%s\n", arg[k]);
                    if (k > 0)
                    {
                        printf("\n");
                    }
                    continue;
                }
                if (arg[1] != NULL && checkiffile(arg[k]) != 1)
                {
                    printf("%s:\n", arg[k]);
                }
                if (arg[1] != NULL && checkiffile(arg[k]) == 1)
                {
                    printf("%s\n", arg[k]);
                }
                if (ol == 0 && oa == 0)
                {
                    lsafunc(arg[k], 0, arg[k]);
                    if (k > 0)
                    {
                        printf("\n");
                    }
                    continue;
                }
                if (oa == 1 && ol != 1)
                {
                    lsafunc(arg[k], 1, arg[k]);
                    if (k > 0)
                    {
                        printf("\n");
                    }
                    continue;
                }
                else if (ol == 1)
                {
                    if (oa == 1)
                    {
                        lslfunc(arg[k], 1, arg[k]);
                        if (k > 0)
                        {
                            printf("\n");
                        }
                    }
                    else
                    {
                        lslfunc(arg[k], 0, arg[k]);
                        if (k > 0)
                        {
                            printf("\n");
                        }
                    }
                }
            }
            return;
        }
    }
}
