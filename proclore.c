#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include<unistd.h>

#include"proclore.h"


void proclore(int count, char commands[][100]){
    if(count == 0){
        int shell_pid = getpid(); // Get the current process ID
        char path[256];
        sprintf(path, "/proc/%d/status", shell_pid); // Construct the path to the status file

        FILE *fp = fopen(path, "r");
        if (fp == NULL) {
            perror("Failed to open file");
            return;
        }

        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            if (strncmp(line, "State:", 6) == 0) {
                printf("Process State: %s", line);
            } else if (strncmp(line, "PPid:", 5) == 0) {
                printf("Parent PID: %s", line);
            } else if (strncmp(line, "Gid:", 4) == 0) {
                printf("Group ID: %s", line);
            } else if (strncmp(line, "VmSize:", 7) == 0) {
                printf("Virtual Memory Size: %s", line);
            }
        }

        fclose(fp);
    }
    else if(count == 1){
        int pid = atoi(commands[1]);
        char path[256];
        sprintf(path, "/proc/%d/status", pid); // Construct the path to the status file

        FILE *fp = fopen(path, "r");
        if (fp == NULL) {
            perror("Failed to open file");
            return;
        }

        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            if (strncmp(line, "State:", 6) == 0) {
                printf("Process State: %s", line);
            } else if (strncmp(line, "PPid:", 5) == 0) {
                printf("Parent PID: %s", line);
            } else if (strncmp(line, "Gid:", 4) == 0) {
                printf("Group ID: %s", line);
            } else if (strncmp(line, "VmSize:", 7) == 0) {
                printf("Virtual Memory Size: %s", line);
            }
        }

        fclose(fp);
    }
    
    
}

