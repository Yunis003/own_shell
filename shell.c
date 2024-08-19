#include "main.h"

/**
  * main - Getline function
  * @argc: Argument count
  * @argv: Array of argument values
  *
  * Return: 0 on success
  */
int main()
{
    char *buf = NULL;
    char *token;
    size_t count = 0;
    ssize_t nread;
    pid_t child_pid;
    int i, status;
    char **array;
    int pipefd[2];
    int has_pipe = 0;
    char *cmd2[1024];
    int k = 0;

    while (1)
    {
        nread = getline(&buf, &count, stdin);

        if (nread == -1)
        {
            perror("Exiting shell");
            exit(1);
        }

        buf[strlen(buf) - 1] = '\0';

        for (i = 0; buf[i]; i++)
        {
            if (buf[i] == '|')
            {
                has_pipe = 1;
                buf[i] = '\0';
                break;
            }
        }

        token = strtok(buf, " ");
        array = malloc(sizeof(char*) * 1024);
        i = 0;

        while (token)
        {
            array[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        array[i] = NULL;

        if (has_pipe)
        {
            token = strtok(buf + i + 1, " ");
            while (token)
            {
                cmd2[k] = token;
                token = strtok(NULL, " ");
                k++;
            }
            cmd2[k] = NULL;
        }

        child_pid = fork();

        if (child_pid == -1)
        {
            perror("Failed to create.");
            exit(41);
        }

        if (child_pid == 0)
        {
            if (has_pipe)
            {
                if (pipe(pipefd) == -1)
                {
                    perror("Pipe failed");
                    exit(1);
                }

                if (fork() == 0)
                {
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[0]);
                    close(pipefd[1]);
                    execvp(array[0], array);
                    perror("Exec failed");
                    exit(1);
                }
                else
                {
                    wait(NULL);
                    dup2(pipefd[0], STDIN_FILENO);
                    close(pipefd[1]);
                    close(pipefd[0]);
                    execvp(cmd2[0], cmd2);
                    perror("Exec failed");
                    exit(1);
                }
            }
            else
            {
                execvp(array[0], array);
                perror("Exec failed");
                exit(1);
            }
        }
        else
        {
            wait(&status);
        }

        free(array);
        has_pipe = 0;
    }

    free(buf);
    return 0;
}
