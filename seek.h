#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>


#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RESET "\033[0m"


int is_directory(const char *path);
void print_file_contents(const char *file_path);
void print_path_with_color(const char *path, int is_dir);
int process_entry(const char *flags, const char *search, const char *full_path, int is_dir);
int seek_recursive(const char *flags, const char *search, const char *directory);
void seek(const char *flags, const char *search, const char *target_directory);
void parse_input(char *input, char *flags, char *search, char *target_directory, const char *prevDir, const char *homeDir);
void call_seek(char *input, const char *prevDir, const char *homeDir);


