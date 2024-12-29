#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern char **environ;

char *get_command_path(char *cmd) {
    char *path_env;
    char *path;
    char *dir;
    char *full_path;

    path_env = getenv("PATH");
    if (path_env == NULL) {
        return NULL;
    }

    path = strdup(path_env);
    dir = strtok(path, ":");

    while (dir != NULL) {
        full_path = malloc(strlen(dir) + strlen(cmd) + 2);

        if (full_path == NULL) {
            free(path);
            return NULL;
        }

        sprintf(full_path, "%s/%s", dir, cmd);

        if (access(full_path, X_OK) == 0) {
            free(path);
            return full_path;
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path);
    return NULL;
}

int main(void) {
    char *buffer = NULL;
    char *newline;
    char *cmd;
    size_t bufsize = 0;
    int i;
    char *args[64];
    char *arg;
    char *full_cmd_path;
<<<<<<< HEAD
    int status = 0;

    buffer = NULL;
    full_cmd_path = NULL;
=======
>>>>>>> e29c212 (Update)

    while (1) {
        printf("$ ");
        if (getline(&buffer, &bufsize, stdin) == -1) {
            if (feof(stdin)) {
                break;
            }
            free(buffer);
            continue;
        }

        newline = strchr(buffer, '\n');
        if (newline) {
            *newline = '\0';
        }

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

        i = 0;
        args[i++] = cmd;

        while ((arg = strtok(NULL, " \t\n")) != NULL) {
            args[i++] = arg;
        }
        args[i] = NULL;

        if (cmd[0] == '/' || cmd[0] == '.') {
            if (execve(cmd, args, environ) == -1) {
                perror("execve");
                _exit(EXIT_FAILURE);
            }
        } else {
            full_cmd_path = get_command_path(cmd);
            if (full_cmd_path != NULL) {
                if (execve(full_cmd_path, args, environ) == -1) {
                    perror("execve");
                    free(full_cmd_path);
                    _exit(EXIT_FAILURE);
                }
                free(full_cmd_path);
            } else {
                fprintf(stderr, "%s: command not found\n", cmd);
            }
        }
    }

    free(buffer);
    return 0;
}
