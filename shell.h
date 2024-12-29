#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctype.h>

void execute_command(char *cmd, char **args, char **command);
void handle_exit(char *cmd);
char **pathfinder(char *cmd, char **command);


#endif
