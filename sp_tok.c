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

void append_exec(char *inp, char *home, char *cwd) // append a file.c
{
    // pos off >>
    char *redirect = strstr(inp, ">>");
    if (!redirect)
    {
        printf("Invalid append command!\n");
        return;
    }

    // cmd and file part diff
    char command[256];
    char file[256];

    // copy first part
    strncpy(command, inp, redirect - inp);
    command[redirect - inp] = '\0';

    // coppy second part
    strcpy(file, redirect + 2);
    char *file_trim = strtok(file, " \t\n"); // Trim spcae

    // open file
    int file_fd = open(file_trim, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (file_fd < 0)
    {
        perror("Error opening file for appending");
        return;
    }

    // backup current stdout
    int stdout_backup = dup(STDOUT_FILENO);

    // redirect stdout to the file
    dup2(file_fd, STDOUT_FILENO);

    // tokenize and execute the command part
    char *args[100];
    int count = 0;
    char *token = strtok(command, " ");
    while (token != NULL)
    {
        args[count++] = token;
        token = strtok(NULL, " ");
    }
    args[count] = NULL;

    int pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        execvp(args[0], args);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        wait(NULL);
    }

    // restore stdout to the original file descriptor
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdout_backup);

    close(file_fd);
}

void input_redirect_exec(char *inp, char *home, char *cwd)
{
    char *file_name;
    char command[256];

    // tokenize input to separate the command and the file
    char *token = strtok(inp, "<");
    strcpy(command, token);
    token = strtok(NULL, "< ");
    file_name = token;

    // trim whitespace
    command[strcspn(command, "\n")] = 0;

    // open the file
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        perror("File open failed");
        return;
    }

    // save the original stdin
    int original_stdin = dup(STDIN_FILENO);

    // redirect stdin to the file
    dup2(fileno(file), STDIN_FILENO);

    system(command);

    // restore
    dup2(original_stdin, STDIN_FILENO);

    fclose(file);
}

void pipe_exec(char *inp, char *home, char *cwd)
{
    char *commands[MAX_PIPES + 1];
    int pipefd[MAX_PIPES][2];
    int num_commands = 0;

    // split by |
    char *token = strtok(inp, "|");
    while (token != NULL && num_commands <= MAX_PIPES)
    {
        commands[num_commands++] = token;
        token = strtok(NULL, "|");
    }

    for (int i = 0; i < num_commands - 1; i++)
    {
        // create a pipe for every pair of commands
        if (pipe(pipefd[i]) == -1)
        {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_commands; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            // child process
            if (i > 0)
            {
                dup2(pipefd[i - 1][0], STDIN_FILENO);
                close(pipefd[i - 1][1]);
            }

            if (i < num_commands - 1)
            {
                dup2(pipefd[i][1], STDOUT_FILENO);
                close(pipefd[i][0]);
            }

            for (int j = 0; j < num_commands - 1; j++)
            {
                close(pipefd[j][0]);
                close(pipefd[j][1]);
            }

            char tokens[100][100];
            int count = 0;
            char *arg = strtok(commands[i], " ");
            while (arg != NULL)
            {
                strcpy(tokens[count], arg);
                count++;
                arg = strtok(NULL, " ");
            }

            if (strcmp(tokens[0], "hop") == 0 || strcmp(tokens[0], "reveal") == 0)
            {
                fg(count, tokens, home, cwd);
            }
            else
            {
                execute(count, tokens);
            }

            exit(0);
        }
    }

    for (int i = 0; i < num_commands - 1; i++)
    {
        close(pipefd[i][0]);
        close(pipefd[i][1]);
    }

    for (int i = 0; i < num_commands; i++)
    {
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

    if (strstr(inp, "seek") != NULL)
    {
        char *new_str = inp + 5;
        call_seek(new_str, cwd, home);
        return;
    }
    else if (strstr(inp, ">>") != NULL)
    {
        append_exec(inp, home, cwd); // appending to a file
        return;
    }
    else if (strstr(inp, ">") != NULL)
    {

        char *cmd_part = strtok(inp, ">");
        char *file_part = strtok(NULL, ">");

        // trim psace
        file_part = strtok(file_part, " \t\n");

        FILE *file = fopen(file_part, "w");
        if (!file)
        {
            perror("Error opening file for writing");
            return;
        }

        int stdout_backup = dup(STDOUT_FILENO);
        dup2(fileno(file), STDOUT_FILENO);

        fg_exec(cmd_part, home, cwd);

        dup2(stdout_backup, STDOUT_FILENO);
        close(stdout_backup);
        fclose(file);
        return;
    }
    else if (strstr(inp, "<") != NULL)
    {
        input_redirect_exec(inp, home, cwd);
        return;
    }
    else if (strstr(inp, "|") != NULL)
    {
        pipe_exec(inp, home, cwd);
        return;
    }

    // initial .............................................................................................................

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
        // printf("%d, %s,%s",count,tokens[1],buffer_cwd); //debug
        hop(count - 1, tokens, home, cwd);
        return;
    }
    else if (!strcmp(tokens[0], "reveal"))
    {
        reveal(count - 1, tokens, cwd);
        return;
    }
    else if (!strcmp(tokens[0], "proclore"))
    {
        proclore(count - 1, tokens);
        return;
    }
    else if (!strcmp(tokens[0], "log"))
    {
        main_log(count - 1, tokens);
        return;
    }
    else if (!strcmp(tokens[0], "activities"))
    {
        activities();
        return;
    }
    else if (!strcmp(tokens[0], "bg"))
    {
        ibg(atoi(tokens[1]));
        return;
    }
    else if (!strcmp(tokens[0], "fg"))
    {
        ifg(atoi(tokens[1]));
        return;
    }
    else if (!strcmp(tokens[0], "iman"))
    {
        if (tokens[1] == NULL)
        {
            printf("Invalid command\n");
            return;
        }
        iman(tokens[1]);
        return;
    }
    else if (!strcmp(tokens[0], "ping"))
    {
        if (tokens[1] == NULL)
        {
            printf("Invalid command\n");
            return;
        }
        ping(atoi(tokens[1]), atoi(tokens[2]));
        return;
    }
    else if (!strcmp(tokens[0], "neonate"))
    {
        if (tokens[1] == NULL)
        {
            printf("Invalid command\n");
            return;
        }
        neonate(atoi(tokens[2]));
        return;
    }
    else if (!strcmp(tokens[0], "quit"))
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
}
