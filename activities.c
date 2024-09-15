#include"activities.h"

#define MAX_PROCESSES 100

// Struct to store process information
typedef struct {
    int pid;
    char command[256];
    bool is_running; // true if running, false if stopped
    bool is_ded;
} Process;

Process process_list[MAX_PROCESSES];
int process_count = 0;

// Function to add a process to the process list
void add_process(int pid, const char *command) {
    if (process_count < MAX_PROCESSES) {
        process_list[process_count].pid = pid;
        strcpy(process_list[process_count].command, command);
        process_list[process_count].is_running = true;
        process_list[process_count].is_ded = false;
        process_count++;
    } else {
        printf("Process list full!\n");
    }
}

// Function to update the status of a process (running or stopped)
// void update_process_status(int pid, bool is_running) {
//     for (int i = 0; i < process_count; i++) {
//         if (process_list[i].pid == pid) {
//             process_list[i].is_running = is_running;
//             break;
//         }
//     }
// }

// Comparator function to sort processes lexicographically by command name
int compare_processes(const void *a, const void *b) {
    Process *procA = (Process *)a;
    Process *procB = (Process *)b;
    return strcmp(procA->command, procB->command);
}

// Function to display the list of processes
void activities() {
    for (int i = 0; i < process_count; i++) {
        char proc_path[256];
        snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", process_list[i].pid);

        FILE *proc_file = fopen(proc_path, "r");
        if (proc_file==NULL && errno==ENOENT) {
            // If the file doesn't exist, mark the process as terminated
            process_list[i].is_running = false;
            process_list[i].is_ded = true;
            continue;
        }

        // Read the process state from the file
        char state;
        fscanf(proc_file, "%*d %*s %c", &state);  // Skips fields to read the state
        fclose(proc_file);

        
        if(state=='T'){
            process_list[i].is_running = false; //stopped
        }
        else{
            process_list[i].is_running = true; //running
        }
    }

    // Sort processes lexicographically by command name
    qsort(process_list, process_count, sizeof(Process), compare_processes);

    // Print only processes that are either running or stopped
    for (int i = 0; i < process_count; i++) {
        if (process_list[i].is_running && !process_list[i].is_ded) {
            printf("[%d] : %s - Running\n", process_list[i].pid, process_list[i].command);
        } 
        else if(!process_list[i].is_running && !process_list[i].is_ded){
            printf("[%d] : %s - Stopped\n", process_list[i].pid, process_list[i].command);
        }
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
