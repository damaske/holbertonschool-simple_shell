#include "shell.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

char *find_command_in_path(char *cmd)
{
    char *path_env = getenv("PATH");
    char *path_copy, *directory, *full_path;
    struct stat st;

    if (!path_env)
        return NULL;

    path_copy = strdup(path_env);
    directory = strtok(path_copy, ":");

    while (directory) {
        full_path = malloc(strlen(directory) + strlen(cmd) + 2);
        if (!full_path)
            return NULL;

        sprintf(full_path, "%s/%s", directory, cmd);

        if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR)) {
            free(path_copy);
            return full_path;
        }

        free(full_path);
        directory = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

int main(void)
{
    char *buffer = NULL;
    char *newline;
    char *cmd;
    size_t bufsize = 0;
    pid_t pid;
    int i;
    char *args[100];

    while (1) {
        printf(":) ");
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

        i = 0;

        args[i++] = cmd;

       
        while ((args[i] = strtok(NULL, " \t\n")) != NULL) {
            i++;
        }
        args[i] = NULL;

        if (cmd[0] == '/') {
            if (access(cmd, X_OK) == 0) {
                pid = fork();
                if (pid == 0) {
                    execve(cmd, args, environ);
                    perror("execve");
                    _exit(EXIT_FAILURE);
                } else {
                    wait(NULL);
                }
            } else {
                fprintf(stderr, "./shell: %s: No such file or directory\n", cmd);
            }
        } else {
            char *full_cmd = find_command_in_path(cmd);
            if (full_cmd) {
                pid = fork();
                if (pid == 0) {
                    execve(full_cmd, args, environ);
                    perror("execve");
                    _exit(EXIT_FAILURE);
                } else {
                    wait(NULL);
                }
                free(full_cmd);
            } else {
                fprintf(stderr, "./shell: %s: command not found\n", cmd);
            }
        }
    }

    return 0;
}
