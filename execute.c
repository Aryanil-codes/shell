#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include<fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>

#include"execute.h"
#include"hop.h"
#include"reveal.h"

void execute(int count, char tokens[100][100]) {
    // Null-terminate the token array for execvp
    char *args[count + 2];  // +2 for the command and NULL terminator
    for (int i = 0; i < count; i++) {
        // args[i] = tokens[i];
        args[i]= tokens[i];
        // printf("count - %d\n",count);
        printf("args[%d]: %s\n", i, args[i]);
    }
    args[count] = NULL;  // NULL terminator for execvp
    args[count + 1] = NULL; // Safety NULL pointer at the end

    int fake_pid = fork();
    if (fake_pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (fake_pid == 0) {
        // Child process
        execvp(args[0], args);
        // If execvp fails
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        wait(NULL);
        return;
    }
    return;
}

void bg(int count, char tokens[100][100]) {
    // Null-terminate the token array for execvp
    char *args[count + 2];  // +2 for the command and NULL terminator
    for (int i = 0; i < count; i++) {
        // args[i] = tokens[i];
        args[i]= tokens[i];
        // printf("count - %d\n",count);
        // printf("args[%d]: %s\n", i, args[i]);
    }
    args[count] = NULL;  // NULL terminator for execvp
    args[count + 1] = NULL; // Safety NULL pointer at the end

    int fake_pid = fork();
    if (fake_pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (fake_pid == 0) {
        // Child process
        printf("Background process started with PID %d\n", getpid());
        execvp(args[0], args);
        // If execvp fails
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Parent process waiting for PID %d\n", fake_pid);
        // Parent process
        // wait(NULL);
        return;
    }
}

void fg(int count, char tokens[100][100],char *buffer_cwd,char *current_dir) {

    if (!strcmp(tokens[0],"hop"))
        {
            // printf("%d, %s,%s",count,tokens[1],buffer_cwd);
            hop(count-1,tokens,buffer_cwd,current_dir);
            return;
        }
        else if(!strcmp(tokens[0],"reveal")){
            reveal(count-1,tokens, current_dir);
            return;
        }
        else{
            execute(count,tokens);
            return;
        }


    // Null-terminate the token array for execvp
    char *args[count + 2];  // +2 for the command and NULL terminator
    for (int i = 0; i < count; i++) {
        args[i]= tokens[i];
        // strcpy(args[i],tokens[i]);

        // printf("count - %d\n",count);
        // printf("args[%d]: %s\n", i, args[i]);
    }
    args[count] = NULL;  // NULL terminator for execvp
    args[count + 1] = NULL; // Safety NULL pointer at the end

    int fake_pid = fork();
    if (fake_pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (fake_pid == 0) {
        // Child process
        execvp(args[0], args);
        // If execvp fails
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        wait(NULL);
    }
}
