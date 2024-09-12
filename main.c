#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
// #include <argp.h>

#include "hop.h"
#include "token.h"
#include "reveal.h"
#include "execute.h"
#include "log.h"

#include <fcntl.h>
#include <unistd.h>

void trim_newline(char *str)
{
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

void trim_spaces(char *str)
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

int main()
{

    char buffer_sys[100];
    char buffer_cwd[900];
    char current_dir[900];
    char username[2046];
    char *sysname;
    char inp[1024];

    int hostname = gethostname(buffer_sys, sizeof(buffer_sys));
    if (hostname == -1)
    { 
        perror("something's going wrong with gethostname()");
    }

    int errcode_username = getlogin_r(username, sizeof(username));
    if (errcode_username == -1)
    {
        perror("error in getusername function");
    }

    getcwd(buffer_cwd, sizeof(buffer_cwd));

    while (1)
    {

        getcwd(current_dir, sizeof(current_dir));

        if (strlen(current_dir) < strlen(buffer_cwd))
        {
            printf("\033[0;33m<%s@%s:\033[0;35m%s\033[1;0m>", username, buffer_sys, current_dir);
            fgets(inp, 1024, stdin);
            // printf("Input: %s\n", inp);

            store_log(inp);

            trim_newline(inp);
            trim_spaces(inp);
        }
        else
        {
            char final_cwd[1024];
            int len = strlen(buffer_cwd);
            final_cwd[0] = '~';
            for (int i = len; i < strlen(current_dir); i++)
            {
                final_cwd[i - len + 1] = current_dir[i];
            }
            final_cwd[strlen(current_dir) - len + 2] = '\0';

            printf("\033[0;33m<%s@%s:\033[0;35m%s\033[1;0m>", username, buffer_sys, final_cwd);
            fgets(inp, 1024, stdin);

            store_log(inp);
            
            trim_newline(inp);
            trim_spaces(inp);
            // printf("Input after spaces: %s\n", inp);
            
          ;
        }

        tokenize(inp, current_dir, buffer_cwd);
    }

    return 0;
}