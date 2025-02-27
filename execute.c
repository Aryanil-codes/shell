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
    char *args[count + 1];  
    for (int i = 0; i < count; i++) {
        args[i] = tokens[i];
        // printf("args[%d]: %s\n", i, args[i]);
    }
    args[count] = NULL;  

    int fake_pid = fork();
    if (fake_pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (fake_pid == 0) {
        // Child process
        execvp(args[0], args);
        perror("execvp failed"); 
        exit(EXIT_FAILURE);
    } else {
        
        wait(NULL); 
    }
}

void bg(int count, char tokens[100][100]) {
    char *args[count + 1];  
    for (int i = 0; i < count; i++) {
        args[i] = tokens[i];
    }
    args[count] = NULL;  

    int fake_pid = fork();
    if (fake_pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (fake_pid == 0) {
        printf("Background process started with PID %d\n", getpid());
        execvp(args[0], args);
        perror("execvp failed");  
        exit(EXIT_FAILURE);
    } else {
        printf("Parent process started background process with PID %d\n", fake_pid);
        
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

