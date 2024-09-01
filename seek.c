#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>   // For directory traversal
#include <sys/stat.h> // For file status

#define GREEN "\033[0;32m"
#define BLUE "\033[0;34m"
#define RESET "\033[0m"

// Function to recursively search directories
void search_directory(const char *dir_path, const char *target, bool search_files, bool search_dirs, bool exact, bool *found, bool *is_dir_found) {
    struct dirent *entry;
    DIR *dp = opendir(dir_path);

    if (dp == NULL) {
        perror("opendir failed");
        return;
    }

    char path[1024];

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
        struct stat path_stat;
        stat(path, &path_stat);

        if (S_ISDIR(path_stat.st_mode)) {
            // It's a directory
            if (strcmp(entry->d_name, target) == 0 && search_dirs) {
                printf(BLUE "%s\n" RESET, path + strlen(dir_path) + 1); // Print relative path
                *found = true;
                *is_dir_found = true;
                if (exact) {
                    if (access(path, X_OK) == 0) { // Check execute permission
                        chdir(path); // Change directory
                        printf("Changed directory to: %s\n", path);
                    } else {
                        printf("Missing permissions for task!\n");
                    }
                    closedir(dp);
                    return;
                }
            }

            search_directory(path, target, search_files, search_dirs, exact, found, is_dir_found);
        } else {
            // It's a file
            if (strcmp(entry->d_name, target) == 0 && search_files) {
                printf(GREEN "%s\n" RESET, path + strlen(dir_path) + 1); // Print relative path
                *found = true;
                if (exact) {
                    if (access(path, R_OK) == 0) { // Check read permission
                        FILE *file = fopen(path, "r");
                        if (file != NULL) {
                            char ch;
                            while ((ch = fgetc(file)) != EOF) {
                                putchar(ch);
                            }
                            fclose(file);
                        }
                    } else {
                        printf("Missing permissions for task!\n");
                    }
                    closedir(dp);
                    return;
                }
            }
        }
    }

    closedir(dp);
}

void seek(int count, char commands[][100]) {
    bool search_files = true, search_dirs = true, exact = false;
    char *target = NULL;
    char *dir = ".";  // Default to current directory

    if (count == 0) {
        printf("Usage: seek <flags> <search> <target_directory>\n");
        return;
    }

    int target_index = 1;
    
    // Parse flags
    for (int i = 0; i < count; i++) {
        if (commands[i][0] == '-') {
            if (strcmp(commands[i], "-d") == 0) {
                search_files = false;
            } else if (strcmp(commands[i], "-f") == 0) {
                search_dirs = false;
            } else if (strcmp(commands[i], "-e") == 0) {
                exact = true;
            } else {
                printf("Invalid flag: %s\n", commands[i]);
                return;
            }
        } else {
            target_index = i;
            target = commands[i];
            break;
        }
    }

    if (!search_files && !search_dirs) {
        printf("Invalid flags!\n");
        return;
    }

    if (target == NULL) {
        printf("No target specified!\n");
        return;
    }

    if (count > target_index + 1) {
        dir = commands[target_index + 1];
    }

    bool found = false;
    bool is_dir_found = false;
    search_directory(dir, target, search_files, search_dirs, exact, &found, &is_dir_found);

    if (!found) {
        printf("No match found!\n");
    }
}
