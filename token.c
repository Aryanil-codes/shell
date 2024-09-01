#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "token.h"
#include "sp_tok.h"

void trim_newline2(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

void trim_spaces2(char *str)
{
    char *start = str;
    char *end = str + strlen(str);

    // Trim leading spaces
    while (isspace((unsigned char)*start))
    {
        start++;
    }

    // Trim trailing spaces
    while (end > start && isspace((unsigned char)*end))
    {
        end--;
    }

    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    // Move the trimmed string to the start
    if (start != str)
    {
        memmove(str, start, (end - start + 1));
    }
}


void tokenize(char *inp, char *cwd, char* home)
{
    printf("Input: %s\n", inp);

    char temp_inp[4096];
    char *saveptr;

    strcpy(temp_inp, inp);
    char *token = strtok_r(temp_inp, ";", &saveptr);
    char temp[4096];
    while (token != NULL)
    {
        strcpy(temp, token);
        // trim_newline2(temp);
        printf("Line: %s\n", temp);
        trim_spaces2(temp);
        // temp[strlen(temp)-1]='\0'; // maniac
        printf("Line: %s\n", temp);
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

void amp_tokenize(char *line, char* cwd, char* home)
{
    int count = count_char(line, '&');
    printf("Num BG: %d\n", count);

    char *saveptr;
    char *token = strtok_r(line, "&", &saveptr);
    while (token != NULL)
    {
        char command[4096];
        strcpy(command, token);
        printf("Command: %s\n", command);
        if (count > 0)
        {
            printf("BG: ");
            bg_exec(command);
            count--;
        }
        else
        {
            fg_exec(command, home, cwd);
        }
        token = strtok_r(NULL, "&", &saveptr);
    }
}

// // Assume these functions are defined elsewhere
// void bg_exec(const char *command) {
//     // Background execution implementation
// }

// void fg_exec(const char *command, char* home, char* cwd) {
//     // Foreground execution implementation
// }

// int main() {
//     char input[] = "command1; command2 & command3; command4";
//     char cwd[] = "/current/working/directory";
//     char home[] = "/home/user";

//     tokenize(input, cwd, home);

//     return 0;
// }
