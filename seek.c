#include "seek.h"

// Other necessary includes

#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

int is_directory(const char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) != 0)
    {
        fprintf(stderr, "Error accessing %s: %s\n", path, strerror(errno));
        return 0;
    }
    return S_ISDIR(path_stat.st_mode);
}

void print_file_contents(const char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (!file)
    {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }

    fclose(file);
}

void print_path_with_color(const char *path, int is_dir)
{
    printf("%s%s%s\n", is_dir ? BLUE : GREEN, path, RESET);
}

int process_entry(const char *flags, const char *search, const char *full_path, int is_dir)
{
    int execute_flag = strchr(flags, 'e') != NULL;
    int match_found = strcmp(search, full_path + strlen(full_path) - strlen(search)) == 0;

    if (match_found)
    {
        print_path_with_color(full_path, is_dir);
        if (execute_flag && !is_dir)
        { // Execute flag only applies to files
            print_file_contents(full_path);
        }
        return 1; // Always return 1 if a match is found
    }

    return 0; // Return 0 if no match
}

int seek_recursive(const char *flags, const char *search, const char *directory)
{
    DIR *dir = opendir(directory);
    if (!dir)
    {
        fprintf(stderr, "Error opening directory %s: %s\n", directory, strerror(errno));
        return 0;
    }

    struct dirent *entry;
    char full_path[4096];
    int match_count = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(full_path, sizeof(full_path), "%s/%s", directory, entry->d_name);
        int is_dir = is_directory(full_path);

        match_count += process_entry(flags, search, full_path, is_dir);

        if (is_dir && !strchr(flags, 'f'))
        {
            match_count += seek_recursive(flags, search, full_path);
        }
    }

    closedir(dir);
    return match_count;
}

void seek(const char *flags, const char *search, const char *target_directory)
{
    char resolved_directory[4096];
    realpath(target_directory, resolved_directory);

    int match_count = seek_recursive(flags, search, resolved_directory);

    if (match_count == 0)
    {
        printf("\033[31mNo match found!\033[0m\n");
    }
    else
    {
        printf("\033[32m%d matches found!\033[0m\n", match_count);
    }
}

void parse_input(char *input, char *flags, char *search, char *target_directory, const char *prevDir, const char *homeDir)
{
    flags[0] = '\0';
    search[0] = '\0';
    target_directory[0] = '\0';

    char *token = strtok(input, " ");
    int flag_done = 0;

    while (token)
    {
        if (!flag_done && token[0] == '-')
        {
            strcat(flags, token);
        }
        else if (flag_done == 0)
        {
            strcpy(search, token);
            flag_done = 1;
        }
        else
        {
            if (strcmp(token, "~") == 0)
            {
                strcpy(target_directory, homeDir);
            }
            else if (strcmp(token, "-") == 0)
            {
                strcpy(target_directory, prevDir);
            }
            else
            {
                strcpy(target_directory, token);
            }
        }
        token = strtok(NULL, " ");
    }

    if (strlen(target_directory) == 0)
    {
        strcpy(target_directory, ".");
    }
}

void call_seek(char *input, const char *prevDir, const char *homeDir)
{
    char flags[10];
    char search[256];
    char target_directory[4096];

    parse_input(input, flags, search, target_directory, prevDir, homeDir);

    if (strlen(search) == 0)
    {
        printf("\033[31mNo search string provided\033[0m\n");
        return;
    }

    seek(flags, search, target_directory);
}
