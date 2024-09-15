#include "shrc.h"

int alias_count = 0;

void add_alias(char *name, char *command) {
    strcpy(alias_table[alias_count].name, name);
    strcpy(alias_table[alias_count].command, command);
    alias_count++;
}

void parse_myshrc() {
    FILE *file = fopen(".myshrc", "r");
    if (!file) return;

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        char *alias_key = strstr(line, "alias ");
        if (alias_key) {
            alias_key += 6; // Move past "alias "
            char *equals = strchr(alias_key, '=');
            if (equals) {
                *equals = '\0';
                char *command = equals + 1;
                add_alias(alias_key, command);
            }
        }
    }
    fclose(file);
}

char* resolve_alias(char *input) {
    for (int i = 0; i < alias_count; i++) {
        if (strcmp(input, alias_table[i].name) == 0) {
            return alias_table[i].command;
        }
    }
    return input;  // Return the original input if no alias matches
}
