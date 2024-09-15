#include "neonate.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int most_recent_pid()
{
    FILE *loadavg_file;
    char buffer[128];
    int recent_pid = 0;

    loadavg_file = fopen("/proc/loadavg", "r");
    if (!loadavg_file)
    {
        perror("fopen /proc/loadavg");
        return -1;
    }

    // Read the contents of /proc/loadavg
    fgets(buffer, sizeof(buffer), loadavg_file);
    fclose(loadavg_file);

    // Parse the fifth field which is the most recent PID
    sscanf(buffer, "%*s %*s %*s %*s %d", &recent_pid);

    return recent_pid;
}

void set_nonblocking_input()
{
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl F_GETFL");
        exit(1);
    }
    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl F_SETFL");
        exit(1);
    }
}

void reset_nonblocking_input()
{
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl F_GETFL");
        exit(1);
    }
    if (fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK) == -1)
    {
        perror("fcntl F_SETFL");
        exit(1);
    }
}

void neonate(int time_arg)
{
    time_t start_time, current_time;
    char c;

    enableRawMode();
    set_nonblocking_input();

    start_time = 0;

    while (1)
    {
        current_time = time(NULL);

        if (current_time - start_time >= time_arg)
        {
            int pid = most_recent_pid();
            if (pid != -1)
            {
                printf("pid is: %d\n", pid);
            }
            start_time = current_time;
        }

        int n = read(STDIN_FILENO, &c, 1);
        if (n == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                continue;
            }
            else
            {
                perror("read");
                break;
            }
        }
        else if (n == 1 && c == 'x')
        {
            printf("Terminated by 'x' key\n");
            break;
        }
    }

    reset_nonblocking_input();
    disableRawMode();
}
