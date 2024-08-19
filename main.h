#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

char *get_file_path(char *file_name);
char *get_file_loc(char *path, char *file_name);
int startsWithForwardSlash(const char *str);
int handle_builtin_commands(char **array);

#endif /* MAIN_H */

