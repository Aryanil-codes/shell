#include "ping.h"

void ping(int pid, int signal_num) {
    
    int mod_signal = signal_num % 32;

    if (kill(pid, 0) == -1) {
        printf("No such process found\n");
        return;
    }

    // send the signal
    if (kill(pid, mod_signal) == 0) {
        printf("Sent signal %d to process with pid %d\n", mod_signal, pid);
    } else {
        perror("Failed to send signal");
    }
}
