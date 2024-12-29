#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

void execute_command(char *cmd, char **args, char **command);
void handle_exit(char *cmd);
char **pathfinder(char *cmd, char **command);


#endif
