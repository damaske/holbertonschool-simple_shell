#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

char *fullname(char *filename)
{
    char *path;
    static char full_path[1024];
    char path_copy[1024];
    char *dir;
    int existence;

    path = getenv("PATH");
    if (!path)
    {
        perror("Error: PATH not found");
        return NULL;
    }

    strcpy(path_copy, path);
    dir = strtok(path_copy, ":");

    while (dir != NULL)
    {
        if (strlen(dir) + strlen(filename) + 2 > sizeof(full_path))
        {
            fprintf(stderr, "Error: Path too long\n");
            dir = strtok(NULL, ":");
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", dir, filename);

        existence = access(full_path, X_OK);
        if (existence == 0)
        {
            return full_path;
        }
        else
        {
            fprintf(stderr, "Error: Not found or no permission: %s\n", full_path);
        }

        dir = strtok(NULL, ":");
    }

    return NULL;
}

int main(void)
{
    char *buffer = NULL;
    char *newline;
    char *cmd;
    char *cmd_path;
    size_t bufsize = 0;

    while (1) {
        if (getline(&buffer, &bufsize, stdin) == -1) {
            free(buffer);
            break;
        }

        newline = strchr(buffer, '\n');
        if (newline)
            *newline = '\0';

        if (buffer[0] == '\0')
            continue;

        cmd = strtok(buffer, " \t\n");
        if (cmd == NULL)
            continue;

        cmd_path = fullname(cmd);

        if (cmd_path == NULL)
        {
            perror("Command not found");
            continue;
        }

        if (fork() == 0) {
            char *args[3];
            args[0] = cmd_path;
            args[1] = strtok(NULL, " \t\n");
            args[2] = NULL;

            if (execve(cmd_path, args, environ) == -1) {
                perror("execve");
                _exit(EXIT_FAILURE);
            }
        } else {
            wait(NULL);
        }
    }

    return 0;
}
