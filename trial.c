#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int pid = fork();
    if(pid == -1){
        return -1;
    }
    if(pid == 0){
        while(1){
            printf("Child process\n");
            usleep(50000);
        }
    }
    else{
        sleep(1);
        kill(pid, SIGKILL);
        wait(NULL);
    }
    return 0;
}