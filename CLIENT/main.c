#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "socket_handler.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
    int sock_fd = create_client_socket(SERVER_IP, SERVER_PORT);
    if (sock_fd < 0) {
        fprintf(stderr, "שגיאה: לא ניתן היה להתחבר לשרת\n");
        return 1;
    }

    printf("החיבור לשרת בוצע בהצלחה!\n");

    char input[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    while (1) {
        printf("הזן פקודה (או QUIT ליציאה): ");
        fflush(stdout); 
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
            printf("שגיאה בקריאת קלט, מתנתק...\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            continue;
        }

        if (send(sock_fd, input, strlen(input), 0) < 0) {
            perror("send failed");
            break;
        }

        int is_quit = (strcmp(input, "QUIT") == 0);

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("recv failed");
            break;
        }
        if (bytes_received == 0) {
            printf("השרת סגר את החיבור\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Server response: %s\n", buffer);

        if (is_quit) {
            break;
        }
    }

    close(sock_fd);
    printf("החיבור נסגר\n");
    return 0;
}