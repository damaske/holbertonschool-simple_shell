#include "shell.h"

extern char **environ;

char *get_path(void)
{
    char **env = environ;
    char *path = NULL;

    if (env == NULL)
        return (NULL);
    while (*env != NULL)
    {
        if (strncmp(*env, "PATH=", 5) == 0)
        {
            path = *env + 5;
            return (path);
        }
        env++;
    }
    return (NULL);
}

char *get_full_path(char *arg, int *status)
{
    char *PATH;
    char *dir;
    char *full_path;

    if (access(arg, F_OK) == 0)
    {
        full_path = malloc(strlen(arg) + 1);
        strcpy(full_path, arg);
        return (full_path);
    }
    if (get_path() == NULL)
    {
        fprintf(stderr, "./hsh: 1: %s: not found\n", arg);
        *status = 127;
        return (NULL);
    }
    PATH = strdup(get_path());
    dir = strtok(PATH, ":");
    while (dir)
    {
        full_path = malloc(strlen(dir) + strlen(arg) + 2);
        strcpy(full_path, dir);
        strcat(full_path, "/");
        strcat(full_path, arg);
        if (access(full_path, F_OK) == 0)
        {
            free(PATH);
            return (full_path);
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }
    fprintf(stderr, "./hsh: 1: %s: not found\n", arg);
    *status = 127;
    free(PATH);
    return (NULL);
}

int main(void)
{
    char *buffer = NULL;
    char *newline;
    char *cmd;
    size_t bufsize = 0;
    int status = 0;
    char *args[64];
    int i;
    char *arg;
    char *full_path;

    while (1)
    {
        if (getline(&buffer, &bufsize, stdin) == -1)
        {
            free(buffer);
            break;
        }
        
        newline = strchr(buffer, '\n');
        if (newline)
            *newline = '\0';
	if (strcmp(buffer, "exit") == 0)
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

        arg = strtok(NULL, " \t\n");
        while (arg != NULL)
        {
            args[i++] = arg;
            arg = strtok(NULL, " \t\n");
        }
        args[i] = NULL;

        full_path = get_full_path(cmd, &status);
        if (full_path == NULL)
            continue;

        if (fork() == 0)
        {
            if (execve(full_path, args, environ) == -1)
            {
                perror("execve");
                _exit(EXIT_FAILURE);
            }
        }
        else
        {
            wait(&status);
	    if (WIFEXITED(status))
	    {
		    status = WEXITSTATUS(status);
	    }
        }

        free(full_path);
    }

    return 0;
}
