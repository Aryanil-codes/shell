#include "iman.h"

#define BUF_SIZE 4096
#define RESET_COLOR "\x1b[0m"
#define TEXT_RED "\x1b[31m"
#define TEXT_BOLD "\x1b[1m"

void swap_substring(char *text, const char *old_part, const char *new_part)
{
    char temp_buf[BUF_SIZE];
    char *found, *cursor = text;
    int old_len = strlen(old_part);
    int new_len = strlen(new_part);

    temp_buf[0] = '\0'; // empty buffer

    // loop through the string and swap out the old_part with new_part
    while ((found = strstr(cursor, old_part)) != NULL)
    {
        // copy the segment before the match
        strncat(temp_buf, cursor, found - cursor);

        // put the new substring
        strcat(temp_buf, new_part);

        // move the cursor forward after the replaced substring
        cursor = found + old_len;
    }

    // put any remaining part of the string
    strcat(temp_buf, cursor);

    // copy the result back into the original string
    strcpy(text, temp_buf);
}

// removing HTML tags
void print_clean_output(char *content)
{
    int inside_tag = 0; // Track inside an HTML tag

    swap_substring(content, "<br>", "\n");
    swap_substring(content, "<strong>", TEXT_BOLD);
    swap_substring(content, "</strong>", RESET_COLOR);

    for (const char *p = content; *p != '\0'; p++)
    {
        if (*p == '<')
        {
            inside_tag = 1; // enter an HTML tag
        }
        else if (*p == '>')
        {
            inside_tag = 0; // exit an HTML tag
        }
        else if (!inside_tag)
        {
            putchar(*p); // print character only if it's outside of a tag
        }
    }
}

// to send a GET request and display response
void iman(char *command)
{
    if (strlen(command) == 0)
    {
        printf(TEXT_RED "Usage: iMan <command>\n" RESET_COLOR);
        return;
    }

    char cmd_copy[256];
    strncpy(cmd_copy, command, sizeof(cmd_copy) - 1);
    cmd_copy[sizeof(cmd_copy) - 1] = '\0'; // null termination
    char *cmd_token = strtok(cmd_copy, " ");
    if (cmd_token == NULL)
    {
        printf(TEXT_RED "Usage: iMan <command>\n" RESET_COLOR);
        return;
    }

    const char *hostname = "man.he.net";
    const char *url_format = "/?topic=%s&section=all";
    char url_path[256];
    char http_request[512];
    struct sockaddr_in server_address;
    struct hostent *server;
    int sockfd;
    char read_buf[BUF_SIZE];
    int bytes_read;
    int body_started = 0;

    // format the URL path with the command
    snprintf(url_path, sizeof(url_path), url_format, cmd_token);

    // create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // look up the server's DNS entry
    server = gethostbyname(hostname);
    if (server == NULL)
    {
        fprintf(stderr, "Error: No such host\n");
        exit(EXIT_FAILURE);
    }

    // set up the server address structure
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(80);
    memcpy(&server_address.sin_addr.s_addr, server->h_addr, server->h_length); // false error

    // connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Connection error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // create the HTTP GET request
    snprintf(http_request, sizeof(http_request),
             "GET %s HTTP/1.0\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             url_path, hostname);

    // send the GET request
    if (write(sockfd, http_request, strlen(http_request)) < 0)
    {
        perror("Request send error");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // read and process the response
    while ((bytes_read = read(sockfd, read_buf, sizeof(read_buf) - 1)) > 0)
    {
        read_buf[bytes_read] = '\0';

        // skip headers
        if (!body_started)
        {
            char *html_start = strstr(read_buf, "<html>");
            if (html_start != NULL)
            {
                html_start += 4;                // Skip the "\r\n\r\n" part
                print_clean_output(html_start); // Clean up and print the content
                body_started = 1;               // Mark that we're in the body now
            }
        }
        else
        {
            // print the clean content
            print_clean_output(read_buf);
        }
    }

    // close socket
    close(sockfd);
}
