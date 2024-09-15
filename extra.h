#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

extern int fg_prompt;//time
extern char fg_prompt_name[100];//name
extern int fg_pid; //pid

void sigchld_handler(int signum);
void sigint_handler(int sig);
void sigtstp_handler(int sig);

void quit();
