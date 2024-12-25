#include "shell.h"

void execute_command(char *line)
{
    pid_t pid;
    char *token = strtok(line, " ");
    
    while (token != NULL)
    {
        pid = fork();

        if (pid == 0)
        {
            execlp(token, token, NULL);
            perror("execve");
            exit(1);
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        else
        {
            wait(NULL);
        }
        token = strtok(NULL, " ");
    }
}

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while (1)
    {
        printf("#cisfun$ ");
        nread = getline(&line, &len, stdin);

        if (nread == -1)
        {
            free(line);
            exit(0);
        }

        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0)
        {
            continue;
        }

        execute_command(line);
    }

    free(line);
    return 0;
}

