#ifndef SP_TOK_H
#define SP_TOK_H

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
    printf("FG: %s\n", inp);

    if (strstr(inp, "seek") != NULL) {
        char *new_str = inp + 5;
        call_seek(new_str, cwd, home);
        return;
    } 

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
    args[count + 1] = NULL; // Safety NULL pointer at the end

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
        // If execvp fails
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Parent process
        wait(NULL);
    }
}

#endif