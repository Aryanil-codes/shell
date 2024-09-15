#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>

#include "sp_tok.h"
#include "execute.h"
#include "hop.h"
#include "token.h"
#include "reveal.h"
#include "execute.h"
#include "proclore.h"
#include "log.h"
#include "seek.h"
#include "activities.h"
#include "iman.h"
#include "neonate.h"
#include "extra.h"
#include "ping.h"
#include "fgbg.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_PIPES 10

void append_exec(char *inp, char *home, char *cwd) //append a file.c
{
    // Find the position of ">>" in the input string
    char *redirect = strstr(inp, ">>");
    if (!redirect) {
        printf("Invalid append command!\n");
        return;
    }

    // Split the input into command part and file part
    char command[256];
    char file[256];

    // Copy the part before ">>" (the command)
    strncpy(command, inp, redirect - inp);
    command[redirect - inp] = '\0'; // Null-terminate the command string

    // Skip ">>" and any leading spaces, and copy the file name
    strcpy(file, redirect + 2);
    char *file_trim = strtok(file, " \t\n"); // Trim leading/trailing spaces

    // Open the file for appending
    int file_fd = open(file_trim, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (file_fd < 0) {
        perror("Error opening file for appending");
        return;
    }

    // Backup current stdout
    int stdout_backup = dup(STDOUT_FILENO);
    
    // Redirect stdout to the file
    dup2(file_fd, STDOUT_FILENO);

    // Tokenize and execute the command part
    char *args[100];
    int count = 0;
    char *token = strtok(command, " ");
    while (token != NULL) {
        args[count++] = token;
        token = strtok(NULL, " ");
    }
    args[count] = NULL;

    // Fork a new process to execute the command
    int pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // Child process: execute the command
        execvp(args[0], args);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process: wait for the child to finish
        wait(NULL);
    }

    // Restore stdout to the original file descriptor
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdout_backup);

    // Close the file
    close(file_fd);
}

void input_redirect_exec(char *inp, char *home, char *cwd)
{
    char *file_name;
    char command[256];
    
    // Tokenize input to separate the command and the file
    char *token = strtok(inp, "<");
    strcpy(command, token);
    token = strtok(NULL, "< ");
    file_name = token;

    // Trim whitespace from the command string
    command[strcspn(command, "\n")] = 0;

    // Open the file for reading
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("File open failed");
        return;
    }

    // Save the original stdin
    int original_stdin = dup(STDIN_FILENO);

    // Redirect stdin to the file
    dup2(fileno(file), STDIN_FILENO);

    // Execute the command with redirected input
    system(command);

    // Restore the original stdin
    dup2(original_stdin, STDIN_FILENO);

    // Close the file
    fclose(file);
}


void pipe_exec(char *inp, char *home, char *cwd)
{
    char *commands[MAX_PIPES + 1];  // Store commands split by '|'
    int pipefd[MAX_PIPES][2];       // Array to store pipe file descriptors
    int num_commands = 0;

    // Split input by '|'
    char *token = strtok(inp, "|");
    while (token != NULL && num_commands <= MAX_PIPES) {
        commands[num_commands++] = token;
        token = strtok(NULL, "|");
    }

    for (int i = 0; i < num_commands - 1; i++) {
        // Create a pipe for every pair of commands
        if (pipe(pipefd[i]) == -1) {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            if (i > 0) {
                // Not the first command: Redirect input from the previous pipe
                dup2(pipefd[i - 1][0], STDIN_FILENO);
                close(pipefd[i - 1][1]); // Close write end of the previous pipe
            }

            if (i < num_commands - 1) {
                // Not the last command: Redirect output to the next pipe
                dup2(pipefd[i][1], STDOUT_FILENO);
                close(pipefd[i][0]);  // Close read end of this pipe
            }

            // Close all pipes in the child process
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipefd[j][0]);
                close(pipefd[j][1]);
            }

            // Tokenize the command into individual arguments
            char tokens[100][100];
            int count = 0;
            char *arg = strtok(commands[i], " ");
            while (arg != NULL) {
                strcpy(tokens[count], arg);
                count++;
                arg = strtok(NULL, " ");
            }

            // Use fg or execute function to handle the command
            if (strcmp(tokens[0], "hop") == 0 || strcmp(tokens[0], "reveal") == 0) {
                // Handle your own shell commands
                fg(count, tokens, home, cwd); // You can pass the current working directories as needed
            } else {
                // Handle regular commands
                execute(count, tokens);
            }

            exit(0);  // Exit the child process
        }
    }

    // Parent process: Close all pipe file descriptors
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipefd[i][0]);
        close(pipefd[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }
}

// still init...............................................................................................................

void bg_exec(char *inp)
{
    char *token;

    char tokens[100][100];
    int count = 0;
    token = strtok(inp, " ");
    while (token != NULL)
    {
        strcpy(tokens[count], token);
        token = strtok(NULL, " ");
        count++;
    }

    bg(count, tokens);
}

void fg_exec(char *inp, char *home, char *cwd)
{

    if (strstr(inp, "seek") != NULL) {
        char *new_str = inp + 5;
        call_seek(new_str, cwd, home);
        return;
    }
    else if(strstr(inp, ">>") != NULL){
        append_exec(inp, home, cwd);  // appending to a file
        return;
    }
    else if(strstr(inp, ">") != NULL){
        /*manage writing to a file,
        tokenize based on presence of ">" the right side will be the file name
        and on left side we should tokenize and whatever prints on stdout must be redirected on appending to the file*/
        char *cmd_part = strtok(inp, ">");  // Left part before '>' is the command
        char *file_part = strtok(NULL, ">"); // Right part after '>' is the file

        // Trim leading/trailing spaces from file_part if needed
        file_part = strtok(file_part, " \t\n");

        // Redirect output to the file in write mode
        FILE *file = fopen(file_part, "w");
        if (!file) {
            perror("Error opening file for writing");
            return;
        }

        int stdout_backup = dup(STDOUT_FILENO); // Backup current stdout
        dup2(fileno(file), STDOUT_FILENO);      // Redirect stdout to the file

        // Execute the command part
        fg_exec(cmd_part, home, cwd); // Recursively call fg_exec with the command part

        // Restore stdout
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdout_backup);
        fclose(file);
        return;
    }
    else if(strstr(inp, "<") != NULL){
        input_redirect_exec(inp, home, cwd); //< : Reads input from the filename following “<”
        return;
    }
    else if (strstr(inp, "|") != NULL) {
        pipe_exec(inp,home,cwd);  // manage piping
        return;
    }

    //initial .............................................................................................................

    char *token;

    char tokens[100][100];
    int count = 0;
    token = strtok(inp, " ");
    while (token != NULL)
    {
        strcpy(tokens[count], token);
        token = strtok(NULL, " ");
        count++;
    }

    if (!strcmp(tokens[0], "hop"))
    {
        // printf("%d, %s,%s",count,tokens[1],buffer_cwd);
        hop(count - 1, tokens, home, cwd);
        return;
    }
    else if (!strcmp(tokens[0], "reveal"))
    {
        reveal(count - 1, tokens, cwd);
        return;
    }
    else if(!strcmp(tokens[0], "proclore"))
    {
        proclore(count - 1, tokens);
        return;
    }
    // else if(!strcmp(tokens[0], "seek"))
    // {
    //     seek(count - 1, tokens);
    //     return;
    // }
    else if(!strcmp(tokens[0], "log"))
    {
        main_log(count - 1, tokens);
        return;
    }
    else if(!strcmp(tokens[0], "activities"))
    {
        activities();
        return;
    }
    else if(!strcmp(tokens[0],"bg")){
        ibg(atoi(tokens[1]));
        return;
    }
    else if(!strcmp(tokens[0],"fg")){
        ifg(atoi(tokens[1]));
        return;
    }
    else if(!strcmp(tokens[0],"iman"))
    {
        // char empty[100];
        // strcpy(empty,"iman");
        if(tokens[1] == NULL)
        {
            printf("Invalid command\n");
            return;
        }
        iman(tokens[1]);
        return;
    }
    else if(!strcmp(tokens[0],"ping"))
    {
        if(tokens[1] == NULL)
        {
            printf("Invalid command\n");
            return;
        }
        ping(atoi(tokens[1]),atoi(tokens[2]));
        return;
    }
    else if(!strcmp(tokens[0],"neonate"))
    {
        if(tokens[1] == NULL)
        {
            printf("Invalid command\n");
            return;
        }
        neonate(atoi(tokens[2]));
        return;
    }
    else if(!strcmp(tokens[0],"quit"))
    {
        quit();
        return;
    }
    else
    {
        execute(count, tokens);
        return;
    }

    // Null-terminate the token array for execvp
    char *args[count + 2]; // +2 for the command and NULL terminator
    for (int i = 0; i < count; i++)
    {
        // args[i] = tokens[i];
        args[i] = tokens[i];
        // printf("count - %d\n",count);
        // printf("args[%d]: %s\n", i, args[i]);
    }
    args[count] = NULL;     // NULL terminator for execvp
    args[count + 1] = NULL; //safe

    int fake_pid = fork();
    if (fake_pid < 0)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (fake_pid == 0)
    {
        // Child process
        execvp(args[0], args);
    
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        wait(NULL);
    }
}
