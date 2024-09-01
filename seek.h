#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include<unistd.h>


void search_directory(const char *dir_path, const char *target, bool search_files, bool search_dirs, bool exact, bool *found, bool *is_dir_found);
void seek(int count, char commands[][100]);

