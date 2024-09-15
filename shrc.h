#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Alias {
    char name[100];
    char command[100];
} Alias;

extern Alias alias_table[100];  // Array to store aliases



void add_alias(char *name, char *command);

void parse_myshrc() ;

char* resolve_alias(char *input);