#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "log.h"

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 1024

void clean_log() {
    FILE *fptr, *temp;
    fptr = fopen("histfile", "r");
    if (fptr == NULL) {
        perror("Error opening histfile");
        return;
    }

    char buffer[MAX_LINES][MAX_LINE_LENGTH];
    int line_count = 0;

    while(fgets(buffer[line_count], MAX_LINE_LENGTH, fptr) != NULL) {
        line_count++;
    }
    fclose(fptr);

    if(line_count <= 15) {
        return;
    }

    fptr = fopen("histfile", "w");
    if(fptr == NULL) {
        perror("Error opening histfile");
        return;
    }

    for(int i = line_count - 15; i < line_count; i++) {
        fputs(buffer[i], fptr);
    }

    fclose(fptr);
}

void store_log(char *inp) {
    if (strstr(inp, "log") != NULL) {
        return;
    }

    FILE *fptr;
    fptr = fopen("histfile", "r");
    if (fptr == NULL) {
        perror("Error opening histfile");
        return;
    }

    char last_line[MAX_LINE_LENGTH];
    last_line[0] = '\0';


    while (fgets(last_line, MAX_LINE_LENGTH, fptr) != NULL) {
    }
    fclose(fptr);


    if (last_line[strlen(last_line) - 1] == '\n') {
        last_line[strlen(last_line) - 1] = '\0';
    }

    //lastline match
    if (strcmp(last_line, inp) == 0) {
        return;
    }

    fptr = fopen("histfile", "a");
    if (fptr == NULL) {
        perror("Error opening histfile");
        return;
    }

    fprintf(fptr, "%s", inp);
    fclose(fptr);
}

void view_log() {
    FILE *fptr;
    fptr = fopen("histfile", "r");
    if (fptr == NULL) {
        perror("Error opening histfile");
        return;
    }

    char buffer[MAX_LINES][MAX_LINE_LENGTH];
    int line_count = 0;


    while (fgets(buffer[line_count], MAX_LINE_LENGTH, fptr) != NULL) {
        line_count++;
    }
    fclose(fptr);

    int start_line = (line_count > 15) ? line_count - 15 : 0;
    for (int i = start_line; i < line_count; i++) {
        printf("%s", buffer[i]);
    }
}

void log_purge() {
    FILE *fptr;
    fptr = fopen("histfile", "w");
    if (fptr == NULL) {
        perror("Error opening histfile");
        return;
    }
    fclose(fptr);
}

void execute_log(int num) {
    FILE *fptr;
    fptr = fopen("histfile", "r");
    if (fptr == NULL) {
        perror("Error opening histfile");
        return;
    }

    char buffer[MAX_LINES][MAX_LINE_LENGTH];
    int line_count = 0;

    while (fgets(buffer[line_count], MAX_LINE_LENGTH, fptr) != NULL) {
        line_count++;
    }
    fclose(fptr);

    if (num <= 0 || num > line_count) {
        printf("Invalid line number.\n");
        return;
    }

    // bottom exec
    int target_line = line_count - num;
    if (target_line < 0 || target_line >= line_count) {
        printf("Invalid line number.\n");
        return;
    }

    printf("Executing: %s", buffer[target_line]);

    
    if (system(buffer[target_line]) == -1) {
        perror("Error executing command");
    }
}

void main_log(int count, char commands[][100]) {
    if (count == 0) {
        view_log();
    } 
    else if (count == 1) {
        
        log_purge();
        
    } 
    else {
        execute_log(atoi(commands[2]));
    }
}
