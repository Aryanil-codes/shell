#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern int fg_prompt;
extern char fg_prompt_name[100];

void sigchld_handler(int signum);

void quit();
