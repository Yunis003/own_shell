#include "main.h"

/**
 * handle_builtin_commands - Handles built-in commands like echo
 * @array: Command arguments
 *
 * Return: 1 if a built-in command is executed, 0 otherwise
 */
int handle_builtin_commands(char **array)
{
    if (strcmp(array[0], "echo") == 0)
    {
        for (int i = 1; array[i] != NULL; i++)
        {
            printf("%s", array[i]);
            if (array[i + 1] != NULL)
                printf(" ");
        }
        printf("\n");
        return (1);
    }
    return (0);
}

int main()
{
    char *buf = NULL, *token, *path;
    size_t count = 0;
    ssize_t nread;
    pid_t child_pid;
    int i, status;
    char **array;

    while (1)
    {
        printf("$ ");
        nread = getline(&buf, &count, stdin);

        if (nread ==  -1)
        {
            exit(0);
        }

        token = strtok(buf, " \n");

        array = malloc(sizeof(char*) * 1024);
        i = 0;

        while (token)
        {
            array[i] = token;
            token = strtok(NULL, " \n");
            i++;
        }

        array[i] = NULL;

        if (handle_builtin_commands(array))
        {
            free(array);
            continue;
        }

        path = get_file_path(array[0]);

        child_pid = fork();

        if (child_pid == -1)
        {
            perror("Failed to create.");
            exit(41);
        }

        if (child_pid == 0)
        {
            if (execve(path, array, NULL) == -1)
            {
                perror("Failed to execute");
                exit(97);
            }
        }
        else
        {
            wait(&status);
        }

        free(path);
        free(array);
    }
    free(buf);
    return (0);
}

