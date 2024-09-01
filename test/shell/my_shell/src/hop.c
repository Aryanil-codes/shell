#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "hop.h"

void change_directory(const char *path) {
    if (chdir(path) != 0) {
        perror("chdir");
    }
}

void hop(const char *path) {
    if (path == NULL || strcmp(path, "~") == 0) {
        path = getenv("HOME");
    }

    change_directory(path);

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", cwd);
}
