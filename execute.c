#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include<fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#include"execute.h"
#include"hop.h"
#include"reveal.h"

void execute(int count, char tokens[100][100]) {
    char *args[count + 1];  // +1 for NULL terminator
    for (int i = 0; i < count; i++) {
        args[i] = tokens[i];
        printf("args[%d]: %s\n", i, args[i]);
    }
    args[count] = NULL;  // NULL terminator for execvp

    int fake_pid = fork();
    if (fake_pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (fake_pid == 0) {
        // Child process
        execvp(args[0], args);
        perror("execvp failed"); // This will only print if execvp fails
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        wait(NULL);  // Wait for the child process to finish
    }
}

void bg(int count, char tokens[100][100]) {
    char *args[count + 1];  // +1 for NULL terminator
    for (int i = 0; i < count; i++) {
        args[i] = tokens[i];
    }
    args[count] = NULL;  // NULL terminator for execvp

    int fake_pid = fork();
    if (fake_pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (fake_pid == 0) {
        // Child process
        printf("Background process started with PID %d\n", getpid());
        execvp(args[0], args);
        perror("execvp failed");  // This will only print if execvp fails
        exit(EXIT_FAILURE);
    } else {
        printf("Parent process started background process with PID %d\n", fake_pid);
        // Parent process does not wait
    }
}

void fg(int count, char tokens[100][100], char *buffer_cwd, char *current_dir) {
    if (strcmp(tokens[0], "hop") == 0) {
        hop(count - 1, tokens, buffer_cwd, current_dir);
    } else if (strcmp(tokens[0], "reveal") == 0) {
        reveal(count - 1, tokens, current_dir);
    } else {
        execute(count, tokens);
    }
}

