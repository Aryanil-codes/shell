#include"activities.h"

#define MAX_PROCESSES 100

// Struct to store process information
typedef struct {
    int pid;
    char command[256];
    bool is_running; // true if running, false if stopped
} Process;

Process process_list[MAX_PROCESSES];
int process_count = 0;

// Function to add a process to the process list
void add_process(int pid, const char *command) {
    if (process_count < MAX_PROCESSES) {
        process_list[process_count].pid = pid;
        strcpy(process_list[process_count].command, command);
        process_list[process_count].is_running = true;
        process_count++;
    } else {
        printf("Process list full!\n");
    }
}

// Function to update the status of a process (running or stopped)
void update_process_status(int pid, bool is_running) {
    for (int i = 0; i < process_count; i++) {
        if (process_list[i].pid == pid) {
            process_list[i].is_running = is_running;
            break;
        }
    }
}

// Comparator function to sort processes lexicographically by command name
int compare_processes(const void *a, const void *b) {
    Process *procA = (Process *)a;
    Process *procB = (Process *)b;
    return strcmp(procA->command, procB->command);
}

// Function to display the list of processes
void activities() {
    if (process_count == 0) {
        printf("No processes found!\n");
        return;
    }

    // Sort processes lexicographically by command name
    qsort(process_list, process_count, sizeof(Process), compare_processes);

    // Print process details
    for (int i = 0; i < process_count; i++) {
        printf("[%d] : %s - %s\n", process_list[i].pid, process_list[i].command,
               process_list[i].is_running ? "Running" : "Stopped");
        }
}

// Example usage of updating the process list (this would go into your existing process management functions)
// void simulate_process_change() {
//     // Simulate starting a process
//     add_process(221, "emacs new.txt");
//     add_process(430, "vim");
//     add_process(620, "gedit");

//     // Simulate changing process status (vim stopped)
//     update_process_status(430, false);
// }

// // Test main to demonstrate the activities function
// int main() {
//     simulate_process_change();
//     activities();
//     return 0;
// }
