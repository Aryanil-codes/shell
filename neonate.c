// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <termios.h>
// #include <time.h>

// // Function to get the most recent PID from /proc/loadavg
// int get_most_recent_pid() {
//     FILE *loadavg_file;
//     char buffer[128];
//     int recent_pid = 0;

//     loadavg_file = fopen("/proc/loadavg", "r");
//     if (!loadavg_file) {
//         perror("fopen /proc/loadavg");
//         return -1;
//     }

//     // Read the contents of /proc/loadavg
//     fgets(buffer, sizeof(buffer), loadavg_file);
//     fclose(loadavg_file);

//     // Parse the fifth field which is the most recent PID
//     sscanf(buffer, "%*s %*s %*s %*s %d", &recent_pid);

//     return recent_pid;
// }

// // Function to check if 'x' key is pressed (non-blocking)
// int is_x_pressed() {
//     struct termios oldt, newt;
//     int ch;
//     int oldf;

//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;
//     newt.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//     oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
//     fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

//     ch = getchar();

//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     fcntl(STDIN_FILENO, F_SETFL, oldf);

//     if (ch != EOF) {
//         ungetc(ch, stdin);
//         if (ch == 'x') return 1;
//     }

//     return 0;
// }

// // Function to implement the 'neonate -n [time_arg]' command
// void neonate(int time_arg) {
//     struct timespec start, end;
//     long elapsed_time_ms = 0;
//     long interval_ms = time_arg * 1000; // Convert time_arg to milliseconds
//     int last_pid = 0;

//     printf("Press 'x' to terminate the process.\n");

//     // Get the start time
//     clock_gettime(CLOCK_MONOTONIC, &start);

//     while (1) {
//         // Get the most recent PID from /proc/loadavg
//         int recent_pid = get_most_recent_pid();
//         if (recent_pid == -1) {
//             fprintf(stderr, "Failed to get most recent PID\n");
//             return;
//         }

//         // Print the PID only if it's different from the last one printed
//         if (recent_pid != last_pid) {
//             printf("%d\n", recent_pid);
//             last_pid = recent_pid;
//         }

//         // Get the current time and calculate the elapsed time
//         clock_gettime(CLOCK_MONOTONIC, &end);
//         elapsed_time_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;

//         // If the time passed is greater than or equal to time_arg, reset the timer
//         if (elapsed_time_ms >= interval_ms) {
//             clock_gettime(CLOCK_MONOTONIC, &start);  // Reset start time
//         }

//         // Check if 'x' key is pressed to terminate the loop
//         if (is_x_pressed()) {
//             printf("Terminated by 'x' key\n");
//             break;
//         }

//         // Sleep for a short interval to avoid busy-waiting
//         struct timespec sleep_time;
//         sleep_time.tv_sec = 0;
//         sleep_time.tv_nsec = 100000000L;  // 100 milliseconds
//         nanosleep(&sleep_time, NULL);
//     }
// }
//--------------------------------------------------------------------------------------------------------------------------
// int main(int argc, char *argv[]) {
//     if (argc != 3 || strcmp(argv[1], "-n") != 0) {
//         fprintf(stderr, "Usage: %s -n <time_arg>\n", argv[0]);
//         return 1;
//     }

//     int time_arg = atoi(argv[2]);
//     if (time_arg <= 0) {
//         fprintf(stderr, "Invalid time argument. Must be a positive integer.\n");
//         return 1;
//     }

//     neonate(time_arg);
//     return 0;
// }
#include "neonate.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int most_recent_pid() {
    FILE *loadavg_file;
    char buffer[128];
    int recent_pid = 0;

    loadavg_file = fopen("/proc/loadavg", "r");
    if (!loadavg_file) {
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

void set_nonblocking_input() {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        exit(1);
    }
    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(1);
    }
}

void reset_nonblocking_input() {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        exit(1);
    }
    if (fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(1);
    }
}

void neonate(int time_arg) {
    time_t start_time, current_time;
    char c;

    // Enable raw mode and non-blocking input once before the loop
    enableRawMode();
    set_nonblocking_input();

    start_time = 0;

    while (1) {
        current_time = time(NULL);

        // Check if the time interval has passed
        if (current_time - start_time >= time_arg) {
            int pid = most_recent_pid();
            if (pid != -1) {
                printf("pid is: %d\n", pid);
            }
            start_time = current_time;  // Reset the start time
        }

        // Check for 'x' key press in non-blocking input
        int n = read(STDIN_FILENO, &c, 1);
        if (n == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No input available, continue looping
                continue;
            } else {
                perror("read");
                break;
            }
        } else if (n == 1 && c == 'x') {
            // If 'x' key is pressed, terminate the loop
            printf("Terminated by 'x' key\n");
            break;
        }
    }

    // Reset non-blocking input and disable raw mode when done
    reset_nonblocking_input();
    disableRawMode();
}

