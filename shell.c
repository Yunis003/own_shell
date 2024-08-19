#include "main.h"
#include <stdio.h>
/**
  * main - Main entry point for our program
  * @argc: Argument count to the main
  * @argv: Pointer to array of argument values
  *
  * Return: O Always success
  */

int main(int argc, char *argv[])
{
	(void)argc, (void)argv;
        char *buf = NULL, *token;
        size_t count = 0;
        ssize_t nread;
	pid_t my_pid;
	int status, i;
	char **array;

	while (1)
	{
        	write(STDOUT_FILENO, "MyShell$ ", 9);

        	nread = getline(&buf, &count, stdin);

        	if (nread ==  -1)
        	{
                	 perror("Exiting shell");
                 	 exit(1);
        	}

		token = strtok(buf, " \n");

		array = malloc(sizeof(char) * 1000);
		i = 0;

		while (token)
		{
			array[i]  = token;
			token = strtok(NULL, " \n");
			i++;
		}

		array[i] = NULL;

		my_pid = fork();

		if (my_pid == -1)
		{
			perror("Failed to create!");
			exit(41);
		}
		if (my_pid == 0)
		{
			printf("The creation was succesuful\n");
			if (execve(array[0], array, NULL) == -1)
                        {
                                perror("Failed to execute");
                                exit(97);
                        }
		}
		else
		{
			wait(&status);
		}
	}
        free(buf);
        return (0);
}
