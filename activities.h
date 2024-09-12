#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

void add_process(int pid, const char *command);

void update_process_status(int pid, bool is_running); 

int compare_processes(const void *a, const void *b);

void activities();




typedef struct {
    int pid;
    char command[256];
    bool is_running; // true if running, false if stopped
} Process;
