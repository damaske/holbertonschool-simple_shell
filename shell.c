#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

extern char **environ;

char *find_command(char *cmd);

int main(void)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    char *cmd = NULL, *args[1024], *token = NULL;
    pid_t pid;
    int i;

    while (1)
    {
        if (getline(&buffer, &bufsize, stdin) == -1)
        {
            free(buffer);
            break;
        }

        if (buffer[0] == '\n')
            continue;

        buffer[strcspn(buffer, "\n")] = '\0';

        cmd = strtok(buffer, " ");
        if (cmd == NULL)
            continue;

        args[0] = cmd;
        i = 1;
        while ((token = strtok(NULL, " ")) != NULL)
        {
            args[i] = token;
            i++;
        }
        args[i] = NULL;

        cmd = find_command(args[0]);
        if (cmd == NULL)
        {
            fprintf(stderr, "./hsh: %s: Command not found\n", args[0]);
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(cmd);
            free(buffer);
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            if (execve(cmd, args, environ) == -1)
            {
                perror("./hsh");
                free(cmd);
                _exit(EXIT_FAILURE);
            }
        }
        else
        {
            wait(NULL);
            free(cmd);
        }
    }

    free(buffer);
    return 0;
}

char *find_command(char *cmd)
{
    char *path_env = getenv("PATH");
    char *path = NULL, *dir = NULL;
    char *full_path = NULL;
    size_t len;

    if (access(cmd, X_OK) == 0)
        return strdup(cmd);

    if (path_env == NULL || cmd == NULL)
        return NULL;

    path = strdup(path_env);
    if (path == NULL)
        return NULL;

    dir = strtok(path, ":");
    while (dir != NULL)
    {
        len = strlen(dir) + strlen(cmd) + 2;
        full_path = malloc(len);
        if (full_path == NULL)
        {
            free(path);
            return NULL;
        }

        strcpy(full_path, dir);
        strcat(full_path, "/");
        strcat(full_path, cmd);

        if (access(full_path, X_OK) == 0)
        {
            free(path);
            return full_path;
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path);
    return NULL;
}
<<<<<<< HEAD

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
=======
>>>>>>> 5a4b819 (Updat)
