#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include "prompt.h"

void display_prompt() {
    char cwd[1024];
    struct passwd *pw;
    uid_t uid;

    uid = geteuid();
    pw = getpwuid(uid);

    if (pw == NULL) {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    // Simplify the current working directory
    char *home = pw->pw_dir;
    if (strstr(cwd, home) == cwd) {
        printf("<%s@%s:~%s>", pw->pw_name, getenv("HOSTNAME"), cwd + strlen(home));
    } else {
        printf("<%s@%s:%s>", pw->pw_name, getenv("HOSTNAME"), cwd);
    }

    printf(" ");
    fflush(stdout);
}
