#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include<fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>

void execute(int count,char tokens[100][100]);
void bg(int count, char tokens[100][100]);
void fg(int count, char tokens[100][100],char *buffer_cwd,char *current_dir);