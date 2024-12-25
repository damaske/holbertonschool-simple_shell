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

    while (1)
    {
        printf("#cisfun$ ");
	
	nread = getline(&line, &len, stdin);
        if (nread == -1)
	{
            printf("\n");
            break;
        }
	
	if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';

        pid = fork();

        if (pid == -1)
        {
            perror("fork");
            free(line);
            exit(EXIT_FAILURE);
        }
        if (pid == 0) 
	{
            char *args[2];
            args[0] = line;
            args[1] = NULL;
            
	    if (execve(line, args, NULL) == -1)
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
