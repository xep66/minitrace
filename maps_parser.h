# ifndef MAPS_PARSER_H
# define MAPS_PARSER_H

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char	*resolve_path(char *path_pid);
char	**split_word(const char *str, char c);

# endif
