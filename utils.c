#include "shell.h"
#include <stdlib.h>
#include <stdio.h>

extern char **environ;

void handle_exit(void) {
    exit(0);
}

void print_env(void) {
    int i = 0;

    while (environ[i] != NULL) {
        printf("%s\n", environ[i]);
        i++;
    }
}
