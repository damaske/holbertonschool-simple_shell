#include "shell.h"

extern char **environ;

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

        if (fork() == 0) {
            char *args[2];
            args[0] = cmd;
            args[1] = NULL;
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
