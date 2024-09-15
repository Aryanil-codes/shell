#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    char command[256];
    bool is_running; // true if running, false if stopped
    bool is_ded;
} Process;

extern Process process_list[MAX_PROCESSES];

void add_proc(int pid, const char *command);

int compare_processes(const void *a, const void *b);

void run_to_stop(int pid);

void activities();

