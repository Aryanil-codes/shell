#include "extra.h"
#include "activities.h"

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

void sigint_handler(int sig) {
    if (fg_pid > 0) {
        // Send SIGINT to the foreground process group
        if (kill(fg_pid, SIGINT) == -1) {
            perror("Failed to send SIGINT to the foreground process");
        }
    } else {
        printf("\nNo foreground process to interrupt\n");
    }
}

void sigtstp_handler(int sig) {
    if (fg_pid > 0) {
        // Send SIGTSTP to the foreground process group to stop it
        if (kill(fg_pid, SIGTSTP) == -1) {
            perror("Failed to send SIGTSTP to the foreground process");
        } else {
            printf("\nProcess with PID %d pushed to background and stopped\n", fg_pid);
            add_process(fg_pid,fg_prompt_name);
            run_to_stop(fg_pid);
            // Change the state of the process to "Stopped"
            // You may want to keep track of this process for later "fg" (foreground) command
        }
    } else {
        printf("\nNo foreground process to stop\n");
    }
}

void quit(){
    printf("Goodbye!\n");
    exit(0);
}