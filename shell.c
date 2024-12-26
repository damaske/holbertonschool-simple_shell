#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    pid_t pid;
    int i;
    int status;
    char *token;
    char *args[64];

    while (1)
    {
        printf("$ ");
        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            break;
        }
        line[strcspn(line, "\n")] = 0;

        i = 0;
        token = strtok(line, " ");
        while (token != NULL)
        {
            args[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(line);
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            if (execve(args[0], args, NULL) == -1)
            {
                perror("execve");
                free(line);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            wait(&status);
        }
    }

    free(line);
    return 0;
}
