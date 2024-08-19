#include "main.h"

/**
 * main - main function of the shell
 * Return: returns 0 on success
 */

int main(void)
{
    char *command = NULL;
    size_t size = 0;
    char *tmp = NULL;
    int status = 0;
    ssize_t read;

    while (1)
    {
        printf("$ ");
        read = getline(&command, &size, stdin);
        if (read == -1)
        {
            free(command);
            exit(EXIT_FAILURE);
        }
        pre_execute(command, tmp, &status);
    }
    return (0);
}
/**
 * exit_and_env - handle exit and env function
 * @command: command
 * @status: status
 * Return: true if command is env or exit, false otherwise
 */

bool exit_and_env(char *command, int *status)
{
    if (strcmp(command, "env") == 0)
    {
        *status = 0;
        print_env();
        return true;
    }
    if (strcmp(command, "exit") == 0)
    {
        exit(*status);
    }
    return false;
}
