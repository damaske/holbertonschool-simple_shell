#include "shell.h"

extern char **environ;

char **pathfinder(char *cmd, char **command)
{
    char *current_path, *temp_path;
    char *path_tok;
    char *fullpath;
    size_t arglen = strlen(cmd);
    int i;

    fullpath = NULL;

    if (strchr(cmd, '/') != NULL && access(cmd, F_OK) == 0) {
        command[0] = cmd;
        return (command);
    }

    path_tok = NULL;

    current_path = NULL;
    for (i = 0; environ[i] != NULL; i++) {
        if (strncmp(environ[i], "PATH=", 5) == 0) {
            current_path = environ[i] + 5;
            break;
        }
    }

    if (current_path == NULL) {
        return NULL;
    }

    temp_path = strdup(current_path);
    path_tok = strtok(temp_path, ":");

    while (path_tok) {
        fullpath = malloc(arglen + strlen(path_tok) + 2);
        sprintf(fullpath, "%s/%s", path_tok, cmd);
        if (access(fullpath, F_OK) == 0) {
            command[0] = fullpath;
            free(temp_path);
            return (command);
        }
        path_tok = strtok(NULL, ":");
        free(fullpath);
    }
    free(temp_path);
    return (NULL);
}

void handle_exit(char *cmd)
{
    if (cmd != NULL && strcmp(cmd, "exit") == 0) {
        exit(0);
    }
}

void execute_command(char *cmd, char **args, char **command)
{
    if (pathfinder(cmd, command) != NULL) {
        if (fork() == 0) {
            if (execve(command[0], args, environ) == -1) {
                _exit(EXIT_FAILURE);
            }
        } else {
            wait(NULL);
        }
    } else {
        fprintf(stderr, "%s: command not found\n", cmd);
    }
}

int main(void)
{
    char *buffer = NULL;
    char *newline;
    char *cmd;
    size_t bufsize = 0;
    char *args[100];
    char *command[1];
    int i;

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

        handle_exit(cmd);

        args[0] = cmd;
        i = 1;
        while ((args[i] = strtok(NULL, " \t\n")) != NULL) {
            i++;
        }
        args[i] = NULL;

        execute_command(cmd, args, command);
    }

    return 0;
}
