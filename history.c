#include "headers.h"
#include "history.h"

// max function
int max(int a, int b)
{
    int max = a;
    if (b > a)
    {
        return b;
    }
    return a;
}

// function to add history
void add_history(char *line)
{
    if (strcmp(line, "") == 0)
    {
        return;
    }

    FILE *fp;
    fp = fopen(history_file, "a");
    fclose(fp);
    fp = fopen(history_file, "r");
    char *lines[100];

    int n = 0;
    if (fp != NULL)
    {
        lines[n] = malloc(5000);
        size_t s = size;
        while (getline(&lines[n], &s, fp) != -1)
        {
            if (strcmp(lines[n], "\n") != 0)
            {
                lines[++n] = malloc(size);
            }
        }
    }
    fclose(fp);

    fp = fopen(history_file, "w");
    int i;
    for (i = max(0, n - 20); i < n; i++)
    {
        fprintf(fp, "%s", lines[i]);
    }
    char new[size];
    sprintf(new, "%s\n", line);
    if (i == 0 || strcmp(new, lines[i - 1]) != 0)
    {
        fprintf(fp, "%s", new);
    }
    fclose(fp);
    for (i = 0; i <= n; i++)
    {
        free(lines[i]);
    }
}

// function to show ur history
void show_history(int no)
{
    FILE *fp;
    fp = fopen(history_file, "a");
    fclose(fp);
    fp = fopen(history_file, "r");
    char *lines[100];
    int n = 0;
    if (fp != NULL)
    {
        lines[n] = malloc(size);
        size_t s = size;
        while (getline(&lines[n], &s, fp) != -1)
        {
            if (strcmp(lines[n], "\n") != 0)
                lines[++n] = malloc(size);
        }
        fclose(fp);
    }
    int count = 0;
    for (int i = max(0, n - no); i < n; i++)
    {
        printf("%2d  %s", count, lines[i]);
        count++;
        free(lines[i]);
    }
}

