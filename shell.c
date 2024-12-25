#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    pid_t pid;

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

        pid = fork();

        if (pid == 0)
        {
            execlp(line, line, NULL);
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
    }

    free(line);
    return 0;
}

