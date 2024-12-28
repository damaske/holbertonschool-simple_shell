#include "shell.h"

extern char **environ;

void trim_whitespace(char *buffer) {
    size_t start, end;

    start = strspn(buffer, " \t\n\r");
    if (start > 0) {
        memmove(buffer, buffer + start, strlen(buffer) - start + 1);
    }

    end = strlen(buffer) - 1;
    while (end > 0 && isspace((unsigned char)buffer[end])) {
        end--;
    }
    buffer[end + 1] = '\0';
}

int main(void) {
    char *buffer = NULL;
    char *newline;
    size_t bufsize = 0;

    while (1) {
        if (isatty(STDIN_FILENO)) {
            write(STDOUT_FILENO, "$ ", 2);
        }

        if (getline(&buffer, &bufsize, stdin) == -1) {
            free(buffer);
            break;
        }

        newline = strchr(buffer, '\n');
        if (newline) {
            *newline = '\0';
        }

        trim_whitespace(buffer);

        if (buffer[0] == '\0') {
            continue;
        }

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
