#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "socket_handler.h"
#include "validator.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
    int sock_fd = create_client_socket(SERVER_IP, SERVER_PORT);
    if (sock_fd < 0) {
        fprintf(stderr, "Error: Could not connect to server.\n");
        return 1;
    }

    printf("The connection to the server was successful!\n");

    char message[BUFFER_SIZE * 4];
    char command[BUFFER_SIZE];
    char filename[BUFFER_SIZE];
    char content[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    while (1){
        printf("Enter command (CREATE,WRITE, APPEND, READ, DELETE, QUIT): ");
        fflush(stdout);
        if (fgets(command, BUFFER_SIZE, stdin) == NULL) {
            printf("Error reading input, disconnecting...\n");
            break;
        }
        command[strcspn(command, "\n")] = '\0';
        if (strlen(command) == 0) {
            continue;
        }
            if (!is_known_command(command)) {
            printf("Unknown command. Please try again.\n");
            continue;
        }
        if (strcmp(command, "QUIT") == 0) {
            send(sock_fd, command, strlen(command), 0);
            break;
        }
        printf("Enter filename: ");
        fflush(stdout);
        if (fgets(filename, BUFFER_SIZE, stdin) == NULL) {
            printf("Error reading input, disconnecting...\n");
            break;
        }
        filename[strcspn(filename, "\n")] = '\0';

        if (!is_valid_filename(filename)) {
            printf("Invalid filename (empty, starts with '/', or contains '..'). Please try again.\n");
            continue;
        }
        content[0] = '\0';

        if(strcmp(command, "WRITE") == 0 || strcmp(command, "APPEND") == 0) {
            printf("Enter content: ");
            fflush(stdout);
            if (fgets(content, BUFFER_SIZE, stdin) == NULL) {
                printf("Error reading input, disconnecting...\n");
                break;
            }
            content[strcspn(content, "\n")] = '\0';
        }
        snprintf(message, sizeof(message), "%s|%zu|%s|%zu|%s",
                 command, strlen(filename), filename, strlen(content), content);

        if (send(sock_fd, message, strlen(message), 0) < 0) {
           perror("send failed");
           break;
        }
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("recv failed");
            break;
        }
        if (bytes_received == 0) {
            printf("The server closed the connection.\n");
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Server response: %s\n", buffer);
    }
    close(sock_fd);
    printf("The connection is closed.\n");
    return 0;
}