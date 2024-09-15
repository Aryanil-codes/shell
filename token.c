#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "token.h"
#include "sp_tok.h"

void trim_newline2(char *str)
{
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

void trim_spaces2(char *str)
{
    char *start = str;
    char *end = str + strlen(str);

    while (isspace((unsigned char)*start))
    {
        start++;
    }

    while (end > start && isspace((unsigned char)*end))
    {
        end--;
    }

    *(end + 1) = '\0';

    if (start != str)
    {
        memmove(str, start, (end - start + 1));
    }
}

void tokenize(char *inp, char *cwd, char *home)
{
    // printf("Input: %s\n", inp);

    char temp_inp[4096];
    char *saveptr;

    strcpy(temp_inp, inp);
    char *token = strtok_r(temp_inp, ";", &saveptr);
    char temp[4096];
    while (token != NULL)
    {
        strcpy(temp, token);

        trim_spaces2(temp);
        amp_tokenize(temp, cwd, home);
        token = strtok_r(NULL, ";", &saveptr);
    }
}

int count_char(const char *str, char ch)
{
    int count = 0;
    while (*str)
    {
        if (*str == ch)
        {
            count++;
        }
        str++;
    }
    return count;
}

void amp_tokenize(char *line, char *cwd, char *home)
{
    int count = count_char(line, '&');
    // printf("Num BG: %d\n", count); //debug

    char *saveptr;
    char *token = strtok_r(line, "&", &saveptr);
    while (token != NULL)
    {
        char command[4096];
        strcpy(command, token);
        // printf("Command: %s\n", command); //debug
        if (count > 0)
        {
            bg_exec(command);
            count--;
        }
        else
        {
            fg_exec(command, home, cwd);
        }
        token = strtok_r(NULL, "&", &saveptr);
    }
    return;
}
