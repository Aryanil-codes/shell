#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

#define BUFFER_SIZE 1024

char *read_input() {
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
        return NULL;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    char *input = malloc(strlen(buffer) + 1);
    if (input == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(input, buffer);
    return input;
}

void parse_and_execute(char *input) {
    // To be implemented: Tokenize the input and execute commands.
}
