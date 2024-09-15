#include "fgbg.h"

void ifg(int pid) {
    // Send SIGCONT to the background process
    fg_pid = pid;
    if (kill(pid, SIGCONT) == -1) {
        perror("Failed to send SIGCONT signal");
        return;
    }

    // Wait for the process to finish
    int status;
    if (waitpid(pid, &status, WUNTRACED) == -1) {
        perror("Failed to wait for process");
        return;
    }

    // Check if the process was stopped or terminated
    if (WIFEXITED(status)) {
        printf("Process %d exited with status %d\n", pid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Process %d was killed by signal %d\n", pid, WTERMSIG(status));
    } else if (WIFSTOPPED(status)) {
        printf("Process %d was stopped by signal %d\n", pid, WSTOPSIG(status));
    }
}

void ibg(int pid) {
    // Send SIGCONT to the background process
    if (kill(pid, SIGCONT) == -1) {
        perror("Failed to send SIGCONT signal");
        return;
    }

    printf("Resumed background process with PID %d\n", pid);
}