#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "socket_handler.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
    int server_fd = create_server_socket(PORT);
    if (server_fd < 0) {
        fprintf(stderr, "שגיאה: לא ניתן היה ליצור את סוקט השרת\n");
        return 1;
    }

    printf("השרת פועל ומאזין על פורט %d...\n", PORT);

    int client_fd = accept_client(server_fd);
    if (client_fd < 0) {
        fprintf(stderr, "שגיאה: קבלת הלקוח נכשלה\n");
        close(server_fd);
        return 1;
    }

    printf("החיבור עם הלקוח בוצע בהצלחה!\n");

    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE); 

        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("recv failed");
            break;
        }
        if (bytes_received == 0) {
            printf("הלקוח סגר את החיבור\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Received from client: %s\n", buffer);

        if (strcmp(buffer, "QUIT") == 0) {
            const char *response = "Goodbye! Closing connection.";
            send(client_fd, response, strlen(response), 0);
            printf("הלקוח ביקש להתנתק (QUIT)\n");
            break;
        }

        char response[BUFFER_SIZE];
        snprintf(response, BUFFER_SIZE, "Server received: %s", buffer);

        if (send(client_fd, response, strlen(response), 0) < 0) {
            perror("send failed");
            break;
        }
    }

    close(client_fd);
    close(server_fd);
    printf("השרת נסגר\n");

    return 0;
}