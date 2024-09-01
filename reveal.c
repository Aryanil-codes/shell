#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <dirent.h>

#include <unistd.h>

void reveal(int number_of_args, char args[][100], char * cwd)
{
    bool l=false;
    bool a=false;
    // char cwd[4096];
    // getcwd(cwd, sizeof(cwd));
    if (number_of_args == 0)
    {
        DIR *dir = opendir(cwd);
        if (dir == NULL)
        {
            perror("dir failed \n");
        }
        struct dirent *entity;
        entity = readdir(dir);

        while (entity != NULL)
        {
            if (entity->d_name[0] != '.') {
                printf("%s\n", entity->d_name);
            }
            entity = readdir(dir);
        }

        closedir(dir);
    }
    else if (number_of_args == 1)
    {
        if (args[1][0]=='-')
        {
            for (int i = 1; i < strlen(args[1]); i++)
            {
                if (args[1][i]=='l')
                {
                    l = true;
                }
                else if(args[1][i]=='a')
                {
                    printf("it enters this part \n");
                    a = true;
                }
                
            }

            DIR *dir = opendir(cwd);
            if (dir == NULL)
            {
                perror("dir failed \n");
            }
            struct dirent *entity;
            entity = readdir(dir);

            while (entity != NULL)
            {
                printf("%s\n", entity->d_name);   
                entity = readdir(dir);
            }

            closedir(dir);
            
        }
        else
        {
            // char the_target_dir[1024];
            DIR *dir = opendir(args[1]);
            if (dir == NULL)
            {
                perror("dir failed \n");
            }
            struct dirent *entity;
            entity = readdir(dir);

            while (entity != NULL)
            {
                if (entity->d_name[0] != '.') {
                    printf("%s\n", entity->d_name);
                }
                entity = readdir(dir);
            }

            closedir(dir);
        }
    }
    else if(number_of_args>1){
        // for multiple args (repeat)
        int counter = 1;
        if (args[counter][0]=='-')
        {
            while(counter<=number_of_args){
                for (int i = 1; i < strlen(args[counter]); i++)
                {
                    // printf("outside i = %d\n and counter = %d\n\n",i,counter);
                    if (args[counter][i]=='l')
                    {
                        // printf("inside l - counter - %d, i = %d\n",counter,i);
                        l = true;
                    }
                    else if(args[counter][i]=='a')
                    {
                        // printf("it enters this part \n");
                        a = true;
                    }
                    
                }
                counter ++;
            }

            if (a && l)
            {
                printf("both a and l are there\n");
            }
            else if(a || l){
                if(a) printf("only a is there\n");
                else printf("only l is there\n");
            }
            

            DIR *dir = opendir(cwd);
            if (dir == NULL)
            {
                perror("dir failed \n");
            }
            struct dirent *entity;
            entity = readdir(dir);

            while (entity != NULL)
            {
                printf("%s\n", entity->d_name);   
                entity = readdir(dir);
            }

            closedir(dir);
            
        }
    }

}