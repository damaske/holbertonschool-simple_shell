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
    int status;
    char *token;

    while (1)
    {
        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {
            break;
        }

        line[strcspn(line, "\n")] = 0;

        token = strtok(line, " ");
        while (token != NULL)
        {
            pid = fork();
            if (pid == -1)
            {
                perror("fork");
                free(line);
                exit(EXIT_FAILURE);
            }
            if (pid == 0)
            {
                char *args[3];
                args[0] = "./hbtn_ls";
                args[1] = token;
                args[2] = NULL;

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
            token = strtok(NULL, " ");
        }
    }

    free(line);
    return 0;
}
