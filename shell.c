#include "main.h"
/**
  * main - Getline function
  *
  * Return: 0 on success
  */
int main() {
    char *buf = NULL, *token;
    size_t count = 0;
    ssize_t nread;
    pid_t child_pid;
    int i, status;
    char **array;

    while (1) {
        nread = getline(&buf, &count, stdin);

        if (nread == -1) {
            free(buf);
            exit(0);
        }

        if (buf[nread - 1] == '\n') {
            buf[nread - 1] = '\0';
        }

        array = malloc(sizeof(char*) * 1024);
        if (array == NULL) {
            perror("Unable to allocate memory");
            exit(EXIT_FAILURE);
        }

        i = 0;
        token = strtok(buf, " ");

        while (token) {
            array[i] = strdup(token);
            if (array[i] == NULL) {
                perror("Unable to duplicate token");
                exit(EXIT_FAILURE);
            }
            token = strtok(NULL, " ");
            i++;
        }

        array[i] = NULL;

        child_pid = fork();

        if (child_pid == -1) {
            perror("Failed to create.");
            exit(EXIT_FAILURE);
        }
        if (child_pid == 0) {
            if (execve(array[0], array, NULL) == -1) {
                perror("No such file or directory");
                exit(EXIT_FAILURE);
            }
        } else {
            wait(&status);
        }

        for (i = 0; array[i] != NULL; i++) {
            free(array[i]);
        }
        free(array);
    }

    free(buf);
    return 0;
}

