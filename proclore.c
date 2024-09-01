#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<errno.h>

#include<unistd.h>

#include"proclore.h"

void get_executable_path(int pid, char *exe_path, int size) {
    char proc_path[2045];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", pid);

    ssize_t len = readlink(proc_path, exe_path, size - 1);
    if (len == -1) {
        fprintf(stderr, "Error reading symbolic link: %s\n", strerror(errno));
        exe_path[0] = '\0'; 
        return;
    }

    exe_path[len] = '\0';  
}

void proclore(int count, char commands[][100]){
    int pid;
    if(count == 0){
        int shell_pid = getpid(); 
        pid=shell_pid;
        char path[256];
        sprintf(path, "/proc/%d/status", shell_pid); 

        FILE *fp = fopen(path, "r");
        if (fp == NULL) {
            perror("Failed to open file");
            return;
        }

        char line[256];
        while (fgets(line, sizeof(line), fp)) {
            if (strncmp(line, "State:", 6) == 0) {
                printf("Process State: %s", line);  // remove the extra...trim the line
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
        pid = atoi(commands[1]);
        char path[256];
        sprintf(path, "/proc/%d/status", pid); 

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


    char exe_path[2048];

    get_executable_path(pid, exe_path, sizeof(exe_path));

    if (strlen(exe_path) > 0) {
        printf("Executable path for PID %d: %s\n", pid, exe_path);
    } else {
        printf("Could not retrieve executable path for PID %d\n", pid);
    }
    
    
}

