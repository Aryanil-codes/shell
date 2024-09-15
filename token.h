#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

void tokenize(char *inp, char *cwd, char *home);
void amp_tokenize(char *line, char *cwd, char *home);
