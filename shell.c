#include "main.h"

/**
 * execute_command - func for executing command
 * @args: arguments to path
 * @path: path
 */
void execute_commands(char **args, char *path)
{
	pid_t pid;
	int ststaus;


	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
}
	else if (pid == 0)
	{
		if (execve(path, args, environ) == -1)
		{
			free(path);
			fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (wait(&ststaus) == -1)
		{
			free(path);
			perror("wait failed");
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * entry_command - func for parsing command
 * @u_command: command to be parsed
 * @args: arguments to command
*/
void entry_command(char *u_command, char **args)
{
	char *command = strtok(u_command, " \t");
	int i = 0;

	args[0] = NULL;
	while (command != NULL && i < MAX_LEN - 1)
	{
		args[i] = command;
		i++;
		command = strtok(NULL, "\t");
	}
	args[i] = NULL;
}

/**
 * devide_commands - commands processor func
 * @commands: commands
 * @commands_array: array for all commands
*/
void devide_commands(char* commands, char** commands_array)
{
	char *command;
	int a = 0;

	command = strtok(commands,"\n");
	while (command != NULL)
	{
		commands_array[a] = command;
		command = strtok(NULL, "\n");
		a++;
	}
	commands_array[a] = NULL;
	}
	
/**
 * handle_commands_array - func for handling array of commands
 * @commands_array: array of commands
 */

void handle_path(char **args, char **path, char **path_env, int *found)
{
	char *path_token = NULL;

	if (*path_env == NULL)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
		free(*path_env);
		free(*path);
		exit(127);
	}
	path_token = strtok(*path_env, ":");

	while (path_token != NULL)
	{
		strcpy(*path, path_token);
		strcat(*path, "/");
		strcat(*path, args[0]);
		if (access(*path, X_OK) != -1)
		{
			*found = 1;
			break;
		}
		path_token = strtok(NULL, ":");
	}
	free(*path_env);
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

void execute_command(char **args, char *path)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (execve(path, args, environ) == -1)
		{
			free(path);
			fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (wait(&status) == -1)
		{
			free(path);
			perror("wait failed");
			exit(EXIT_FAILURE);
		}
	}
}

void handle_commands_array(char **commands_array)
{
	int a = 0;
	char *command;
	if (strcmp(commands_array[a], "exit") == 0)
		exit(0);
	else if (strcmp(commands_array[a], "env") == 0)
		print_env();
	else
		while (commands_array[a] != NULL)
		{
			command = commands_array[a];
			if (strcmp(command, "exit") == 0 && a > 0)
				exit(2);
			handle_command(command);
			a++;
		}
}
/**
 * main - main func to process all functions
 * Return: integer
 */

void parse_command(char *u_command, char **args)
{
	char *command = strtok(u_command, " \t");
	int i = 0;

	args[0] = NULL;
	while (command != NULL && i < MAX_LEN - 1)
	{
		args[i] = command;
		i++;
		command = strtok(NULL, " \t");
	}
	args[i] = NULL;
}

int main (void){
	char command[MAX_LEN];
	char *commands_array[MAX_LEN];

	while (1)
	{
		write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
		if (fgets(command, MAX_LEN, stdin) == NULL)
		{
			if (feof(stdin))
				break;
			perror("Error reading command");
			continue;
		}
		
		command[strcspn(command, "\n")] = '\0';
		devide_commands(command, commands_array);
		handle_command(command);
	}
	return (0);
}
