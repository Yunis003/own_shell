#include "main.h"

/**
 * print_env - function for printing env
 */
void print_env(void)
{
	char **env_ptr = environ;

	while (*env_ptr != NULL)
	{
		printf("%s\n", *env_ptr);
		env_ptr++;
	}
}

/**
 * setup_environment - function for environment setup
 * @path_env: environment path
 * @path: path
 */
void setup_environment(char **path_env, char **path)
{
	int i;

	*path = malloc(MAX_LEN);
	if (!*path)
	{
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}
	memset(*path, 0, MAX_LEN);

	for (i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], PATH, 5) == 0)
		{
			*path_env = strdup(environ[i] + 5);
			break;
		}
	}
}

/**
 * handle_command - command handler
 * @u_command: command
 */
void handle_command(char *u_command)
{
	char *args[MAX_LEN], *path = NULL, *path_env = NULL;
	int found = 0;

	setup_environment(&path_env, &path);
	parse_command(u_command, args);

	if (args[0] == NULL)
	{
		free(path_env);
		free(path);
		return;
	}

	if (strchr(args[0], '/') != NULL)
	{
		if (access(args[0], X_OK) == -1)
		{
			fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
			free(path);
			free(path_env);
			exit(127);
		}
		free(path);
		free(path_env);
		path = strdup(args[0]);
		found = 1;
	}
	else
		handle_path(args, &path, &path_env, &found);

	if (found == 0)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
		free(path);
		exit(127);
	}

	execute_command(args, path);
	free(path);
}

/**
 * main - main func to process all functions
 * Return: integer
 */
int main(void)
{
	char commands[MAX_LEN];
	char *commands_array[MAX_LEN];
	ssize_t read_size;

	while (1)
	{
		read_size = read(STDIN_FILENO, commands, MAX_LEN);
		if (read_size == -1)
		{
			perror("Error reading command");
			exit(EXIT_FAILURE);
		}
		else if (read_size == 0)
			break;
		commands[read_size] = '\0';

		process_commands(commands, commands_array);
		handle_commands_array(commands_array);
	}
	return (0);
}

