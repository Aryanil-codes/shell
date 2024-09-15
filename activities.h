#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

void add_process(int pid, const char *command);

void update_process_status(int pid, bool is_running); 

int compare_processes(const void *a, const void *b);

void activities();




struct Process;