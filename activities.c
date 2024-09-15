#include "activities.h"

Process process_list[MAX_PROCESSES];
int process_count = 0;

void add_proc(int pid, const char *command)
{
    if (process_count < MAX_PROCESSES)
    {
        process_list[process_count].pid = pid;
        strcpy(process_list[process_count].command, command);
        process_list[process_count].is_running = true;
        process_list[process_count].is_ded = false;
        process_count++;
    }
    else
    {
        printf("Process list full!\n");
    }
}

void run_to_stop(int pid)
{
    for (int i = 0; i < process_count; i++)
    {
        if (process_list[i].pid == pid)
        {
            process_list[i].is_running = false;
            break;
        }
    }
    return;
}
int compare_processes(const void *a, const void *b)
{
    Process *procA = (Process *)a;
    Process *procB = (Process *)b;
    return strcmp(procA->command, procB->command);
}

// display the list of processes
void activities()
{
    for (int i = 0; i < process_count; i++)
    {
        char proc_path[256];
        snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", process_list[i].pid);

        FILE *proc_file = fopen(proc_path, "r");
        if (proc_file == NULL && errno == ENOENT)
        {
            // checks ded
            process_list[i].is_running = false;
            process_list[i].is_ded = true;
            continue;
        }

        // Read the process state from the file
        char state;
        fscanf(proc_file, "%*d %*s %c", &state);
        fclose(proc_file);

        if (state == 'T')
        {
            process_list[i].is_running = false; // stopped
        }
        else
        {
            process_list[i].is_running = true; // running
        }
    }

    // lexicographically by command name
    qsort(process_list, process_count, sizeof(Process), compare_processes);

    // Print procs that are unded
    for (int i = 0; i < process_count; i++)
    {
        if (process_list[i].is_running && !process_list[i].is_ded)
        {
            printf("[%d] : %s - Running\n", process_list[i].pid, process_list[i].command);
        }
        else if (!process_list[i].is_running && !process_list[i].is_ded)
        {
            printf("[%d] : %s - Stopped\n", process_list[i].pid, process_list[i].command);
        }
    }
}
