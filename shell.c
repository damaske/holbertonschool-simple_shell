#include "shell.h"

extern char **environ;

int main(void) {
    char *buffer = NULL;
            char *newline;
    size_t bufsize = 0;

    while (1) {
        write(STDOUT_FILENO, "$ ", 2);
        if (getline(&buffer, &bufsize, stdin) == -1) {
            write(STDOUT_FILENO, "\n", 1);
            free(buffer);
            break;
        }

        newline = strchr(buffer, '\n');
        if (newline) *newline = '\0';
        if (buffer[0] == '\0') continue;

        if (fork() == 0) {
            char *args[2];
            args[0] = buffer;
            args[1] = NULL;
            execve(buffer, args, environ);
            perror("execve");
            _exit(EXIT_FAILURE);
        } else {
            wait(NULL);
        }
    }

    return 0;
}