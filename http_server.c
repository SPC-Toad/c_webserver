#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h> // For close()

int main(int argc, char* argv[]) {
    // Take in the port number from command line argument.
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }
    int port = atoi(argv[1]);
    printf("You have entered port: %d\n", port);

    // Create or initialize the connection on a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Failed to create socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind socket");
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, 10) < 0) {
        perror("Failed to listen on socket");
        close(server_socket);
        return 1;
    }

    int client_socket;
    while (1) {
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Failed to accept connection");
            close(server_socket);
            return 1;
        }

        // Read the content of index.html each time a request is received
        FILE *file = fopen("index.html", "r");
        if (file == NULL) {
            perror("Failed to open file");
            close(client_socket);
            continue;
        }

        // Get the size of the file
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Allocate memory for the file content
        char *server_response = malloc(file_size + 1);
        if (server_response == NULL) {
            perror("Failed to allocate memory");
            fclose(file);
            close(client_socket);
            continue;
        }

        // Read the file into the allocated memory
        fread(server_response, sizeof(char), file_size, file);
        fclose(file);
        server_response[file_size] = '\0'; // Null-terminate the string

        // Create HTTP response
        const char *http_header_template = 
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %ld\r\n" // Content-Length header
            "\r\n";
        
        char http_header[256];
        snprintf(http_header, sizeof(http_header), http_header_template, file_size);

        size_t response_len = strlen(http_header) + file_size;
        char *http_response = malloc(response_len + 1);
        if (http_response == NULL) {
            perror("Failed to allocate memory for HTTP response");
            free(server_response);
            close(client_socket);
            continue;
        }

        strcpy(http_response, http_header);
        strcat(http_response, server_response);

        send(client_socket, http_response, response_len, 0);

        // Clean up
        free(server_response);
        free(http_response);
        close(client_socket);
    }

    close(server_socket);
    return 0;
}
