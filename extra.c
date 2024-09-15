#include "extra.h"

void sigchld_handler(int signum) {
    int status;
    pid_t pid;

    // Wait for any child process that has terminated
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("\nBackground process with PID %d finished with exit status %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("\nBackground process with PID %d was terminated by signal %d\n", pid, WTERMSIG(status));
        }
        fflush(stdout);  // Ensure output is printed immediately
    }
}

void quit(){
    printf("Goodbye!\n");
    exit(0);
}