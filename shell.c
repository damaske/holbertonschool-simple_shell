#include "shell.h"

extern char **environ;

char *find_command_in_path(char *cmd)
{
    char *path;
    char *path_copy;
    char *dir;
    struct stat st;
    char *full_path;

    int i;
    path = NULL;
    for (i = 0; environ[i] != NULL; i++) {
        if (strncmp(environ[i], "PATH=", 5) == 0) {
            path = environ[i] + 5;
            break;
        }
    }

    if (!path)
        return NULL;

    path_copy = strdup(path);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");

    while (dir) {
        full_path = malloc(strlen(dir) + strlen(cmd) + 2);
        if (!full_path) {
            free(path_copy);
            return NULL;
        }

        strcpy(full_path, dir);
        strcat(full_path, "/");
        strcat(full_path, cmd);

        if (stat(full_path, &st) == 0) {
            free(path_copy);
            return full_path;
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

int main(void)
{
    char *buffer;
    char *newline;
    char *cmd;
    size_t bufsize = 0;
    char *full_cmd_path;
    int status = 0;

    buffer = NULL;
    full_cmd_path = NULL;

    while (1) {
        if (getline(&buffer, &bufsize, stdin) == -1) {
            free(buffer);
            break;
        }

        newline = strchr(buffer, '\n');
        if (newline)
            *newline = '\0';

	if (strcmp(buffer , "exit") == 0)
	{
		free(buffer);
		exit(status);
	}

        if (buffer[0] == '\0')
            continue;

        cmd = strtok(buffer, " \t\n");
        if (cmd == NULL)
            continue;

        if (cmd[0] != '/' && cmd[0] != '.') {
            full_cmd_path = find_command_in_path(cmd);
        } else {
            full_cmd_path = strdup(cmd);
        }

        if (full_cmd_path) {
            if (fork() == 0) {
                char *args[3];
                args[0] = full_cmd_path;
                args[1] = strtok(NULL, " \t\n");
                args[2] = NULL;

                if (execve(full_cmd_path, args, environ) == -1) {
                    perror("execve");
                    _exit(EXIT_FAILURE);
                }
            } else {
                wait(&status);
            }
            free(full_cmd_path);
        } else {
            fprintf(stderr, "%s: command not found\n", cmd);
        }
    }

    return 0;
}
