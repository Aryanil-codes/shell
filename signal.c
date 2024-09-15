// #include <signal.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>

// // Global variable to track the PID of the current foreground process
// int fg_pid = -1;

// // Signal handler for SIGINT (Ctrl-C)
// void sigint_handler(int sig) {
//     if (fg_pid > 0) {
//         // Send SIGINT to the foreground process
//         kill(fg_pid, SIGINT);
//         printf("\nSent SIGINT to process with PID %d\n", fg_pid);
//     } else {
//         printf("\nNo foreground process to interrupt.\n");
//     }
// }
