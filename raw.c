// #include <termios.h>
// #include <unistd.h>

// struct termios orig_termios;

// void disable_raw_mode() {
//     tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
// }

// void enable_raw_mode() {
//     struct termios raw;

//     tcgetattr(STDIN_FILENO, &orig_termios);
//     raw = orig_termios;
//     raw.c_lflag &= ~(ECHO | ICANON | ISIG); // Disable echo, canonical mode, and signal characters (Ctrl-C, Ctrl-Z, etc.)

//     tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
// }

// void raw_mode_input() {
//     char c;
//     while (1) {
//         if (read(STDIN_FILENO, &c, 1) == -1) {
//             perror("read");
//             exit(1);
//         }

//         // Ctrl-C (ASCII 3) sends SIGINT to the foreground process
//         if (c == 3) { // ASCII value of Ctrl-C is 3
//             if (fg_pid != -1) {
//                 kill(fg_pid, SIGINT);
//                 printf("\nSent SIGINT (Ctrl-C) to process with pid %d\n", fg_pid);
//             }
//         }
//         // Ctrl-Z (ASCII 26) sends SIGTSTP to the foreground process
//         else if (c == 26) { // ASCII value of Ctrl-Z is 26
//             if (fg_pid != -1) {
//                 kill(fg_pid, SIGTSTP);
//                 printf("\nSent SIGTSTP (Ctrl-Z) to process with pid %d, moving it to background\n", fg_pid);
//             }
//         }
//         // Ctrl-D (ASCII 4) exits the shell
//         else if (c == 4) { // ASCII value of Ctrl-D is 4 (EOF)
//             printf("\nExiting shell... Killing all background processes\n");
//             for (int i = 0; i < process_count; i++) {
//                 kill(process_list[i].pid, SIGKILL); // Killing all background processes
//             }
//             exit(0); // Exit the shell
//         }
//         // Normal character input can be processed here
//         else {
//             printf("%c", c);  // Echo the character for demonstration (optional)
//         }
//     }
// }

#include "raw.h"

struct termios orig_termios;   

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// void set_nonblocking_input() {
//     struct termios oldt, newt;
//     int oldf;

//     // Get current terminal attributes
//     tcgetattr(STDIN_FILENO, &oldt);

//     // Copy settings to newt
//     newt = oldt;

//     // Disable canonical mode and echoing
//     newt.c_lflag &= ~(ICANON | ECHO);

//     // Apply the new settings
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);

//     // Set stdin to non-blocking mode
//     oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
//     fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
// }

// void reset_nonblocking_input() {
//     // Get the current file descriptor flags
//     int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
//     if (flags == -1) {
//         // Handle error if needed
//         return;
//     }

//     // Clear the O_NONBLOCK flag to make input blocking again
//     flags &= ~O_NONBLOCK;

//     // Set the modified flags back on the file descriptor
//     if (fcntl(STDIN_FILENO, F_SETFL, flags) == -1) {
//         // Handle error if needed
//         return;
//     }
// }


