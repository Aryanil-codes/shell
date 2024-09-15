#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "hop.h"

char dash_cmd[4096];

void hop(int count, char commands[][100], char *home, char *current_dir)
{
    char cwd[1024];
    printf("hop called\n");

    if (count == 0)
    {
        chdir(home);
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    }
    else if (count == 1)
    {

        getcwd(cwd, sizeof(cwd));
        if (!strcmp("-", commands[1]))
        {
            if (chdir(dash_cmd) == -1)
            {
                perror("chdir failed for dash\n");
            }
            strcpy(dash_cmd, cwd);
            return;
        }
        int x = chdir(commands[1]);
        if (x == -1)
        {
            perror("chdir failed in count == 1\n");
        }

        // printf("%s",commands[1]);
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    }
    else if (count > 1)
    {
        int counter = 1;
        while (counter <= count)
        {
            // this is if "-" comes with the hop as multi arg
            if (!strcmp("-", commands[counter]))
            {
                // printf("skillissue much\n");
                getcwd(cwd, sizeof(cwd));
                if (chdir(dash_cmd) == -1)
                {
                    perror("chdir failed for dash\n");
                }
                strcpy(dash_cmd, current_dir);
                printf("%s\n", dash_cmd);
                counter++;
                continue;
            }

            // normal working without -
            getcwd(cwd, sizeof(cwd));
            strcpy(dash_cmd, cwd);
            int x = chdir(commands[counter]);
            if (x == -1)
            {
                perror("chdir failed in count == 1\n");
            }

            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
            counter++;
        }
    }

    return;
}