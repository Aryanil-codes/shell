#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>

#include<fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

// #include"execute.h"
// #include"hop.h"
// #include"reveal.h"
// #include"activities.h"
// #include"extra.h"

#include "sp_tok.h"
#include "execute.h"
#include "hop.h"
#include "token.h"
#include "reveal.h"
#include "execute.h"
#include "proclore.h"
#include "log.h"
#include "seek.h"
#include "activities.h"
#include "iman.h"
#include "neonate.h"
#include "extra.h"
#include "ping.h"
#include "fgbg.h"
#include "shrc.h"

void execute(int count, char tokens[100][100]) {

    int start_time = time(NULL),endtime =0, status;

    char *args[count + 1];  
    for (int i = 0; i < count; i++) {
        args[i] = tokens[i];
        // printf("args[%d]: %s\n", i, args[i]);
    }
    args[count] = NULL;  

    int fake_pid = fork();
    if (fake_pid < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (fake_pid == 0) {
        // Child process
        execvp(args[0], args);
        fprintf(stderr, "%s is not a valid command\n", args[0]);
        exit(1);
    } else {
        // Parent process
        fg_pid = fake_pid;
        // printf("pid is - %d",fg_pid);
        waitpid(fake_pid, &status, WUNTRACED);
        endtime = time(NULL);
        fg_prompt = endtime - start_time;
        fg_pid = fake_pid;
        // printf("fgpromt - %d\n",fg_prompt);
        strcpy(fg_prompt_name, args[0]);
    }
}

void bg(int count, char tokens[100][100]) {
    char *args[count + 1];  
    for (int i = 0; i < count; i++) {
        args[i] = tokens[i];
    }
    args[count] = NULL;  

    // int child_pid;

    int fake_pid = fork();
    if (fake_pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } 
    else if (fake_pid == 0) {
        // printf("\nBackground process started with PID %d\n", getpid());
        // child_pid = getpid();
        setpgid(0, 0); //setting group pid to differ from fg proc
        execvp(tokens[0], args);
        perror("execvp failed");  
        exit(EXIT_FAILURE);
    } else {
        add_process(fake_pid, args[0]);
        printf("Parent process started background process with PID %d\n", fake_pid);
        signal(SIGCHLD, SIG_IGN); //igone child signal
        signal(SIGCHLD, sigchld_handler);
        
    }
    return;
}

void fg(int count, char tokens[100][100], char *buffer_cwd, char *current_dir) { // may not required!!
    if (strcmp(tokens[0], "hop") == 0) {
        hop(count - 1, tokens, buffer_cwd, current_dir);
    } 
    else if (strcmp(tokens[0], "reveal") == 0) {
        reveal(count - 1, tokens, current_dir);
    }
    else if(!strcmp(tokens[0], "proclore"))
    {
        proclore(count - 1, tokens);
        return;
    }
    // else if(!strcmp(tokens[0], "seek"))
    // {
    //     seek(count - 1, tokens);
    //     return;
    // }
    else if(!strcmp(tokens[0], "log"))
    {
        main_log(count - 1, tokens);
        return;
    }
    else if(!strcmp(tokens[0], "activities"))
    {
        activities();
        return;
    }
    else if(!strcmp(tokens[0],"bg")){
        ibg(atoi(tokens[1]));
        return;
    }
    else if(!strcmp(tokens[0],"fg")){
        ifg(atoi(tokens[1]));
        return;
    }
    else if(!strcmp(tokens[0],"iman"))
    {
        // char empty[100];
        // strcpy(empty,"iman");
        if(tokens[1] == NULL)
        {
            printf("Invalid command\n");
            return;
        }
        iman(tokens[1]);
        return;
    }
    else if(!strcmp(tokens[0],"ping"))
    {
        if(tokens[1] == NULL)
        {
            printf("Invalid command\n");
            return;
        }
        ping(atoi(tokens[1]),atoi(tokens[2]));
        return;
    }
    else if(!strcmp(tokens[0],"neonate"))
    {
        if(tokens[1] == NULL)
        {
            printf("Invalid command\n");
            return;
        }
        neonate(atoi(tokens[2]));
        return;
    }
    else if(!strcmp(tokens[0],"quit"))
    {
        quit();
        return;
    }
    else {
        execute(count, tokens);
    }
}

