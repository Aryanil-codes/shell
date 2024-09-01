#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "prompt.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "system_cmds.h"
#include "proclore.h"
#include "seek.h"

int main() {
    char *input;
    while (1) {
        display_prompt();
        input = read_input();
        if (input == NULL) {
            continue;
        }

        parse_and_execute(input);
        free(input);
    }
    return 0;
}
