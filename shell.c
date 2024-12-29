#include "shell.h"

extern char **environ;

char *get_command_path(char *cmd) {
    char *path = getenv("PATH");
    char *dir = strtok(path, ":");
    char *full_path;
    struct stat st;

    while (dir != NULL) {
        full_path = malloc(strlen(dir) + strlen(cmd) + 2);
        if (!full_path) {
            perror("malloc");
            return NULL;
        }

        sprintf(full_path, "%s/%s", dir, cmd);

        if (stat(full_path, &st) == 0 && S_ISREG(st.st_mode) && access(full_path, X_OK) == 0) {
            return full_path;
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    return NULL;
}

int main(void) {
    char *buffer = NULL;
    char *newline;
    char *cmd;
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

        if (cmd[0] != '/' && cmd[0] != '.') {
            cmd = get_command_path(cmd);
            if (cmd == NULL) {
                perror("command not found");
                continue;
            }
        }

        if (fork() == 0) {
            char *args[3];
            args[0] = cmd;
            args[1] = strtok(NULL, " \t\n");
            args[2] = NULL;
            if (execve(cmd, args, environ) == -1) {
                perror("execve");
                _exit(EXIT_FAILURE);
            }
        } else {
            wait(NULL);
        }
    }

    return 0;
}
