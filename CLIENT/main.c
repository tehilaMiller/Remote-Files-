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
        fprintf(stderr, "שגיאה: לא ניתן היה להתחבר לשרת\n");
        return 1;
    }

    printf("החיבור לשרת בוצע בהצלחה!\n");

    char message[BUFFER_SIZE * 3];
    char command[BUFFER_SIZE];
    char filename[BUFFER_SIZE];
    char content[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    while (1){
        printf("הזן פקודה (CREATE,WRITE, APPEND, READ, DELETE, QUIT): ");
        fflush(stdout);
        if (fgets(command, BUFFER_SIZE, stdin) == NULL) {
            printf("שגיאה בקריאת קלט, מתנתק...\n");
            break;
        }
        command[strcspn(command, "\n")] = '\0';
        if (strlen(command) == 0) {
            continue;
        }
            if (!is_known_command(command)) {
            printf("פקודה לא מוכרת. נסי שוב.\n");
            continue;
        }
        if (strcmp(command, "QUIT") == 0) {
            send(sock_fd, command, strlen(command), 0);
            break;
        }
        printf("הזן שם קובץ: ");
        fflush(stdout);
        if (fgets(filename, BUFFER_SIZE, stdin) == NULL) {
            printf("שגיאה בקריאת קלט, מתנתק...\n");
            break;
        }
        filename[strcspn(filename, "\n")] = '\0';

        if (!is_valid_filename(filename)) {
            printf("שם קובץ לא חוקי (ריק, מתחיל ב-'/', או מכיל '..'). נסי שוב.\n");
            continue;
        }
        content[0] = '\0';

        if(strcmp(command, "WRITE") == 0 || strcmp(command, "APPEND") == 0) {
            printf("הזן תוכן : ");
            fflush(stdout);
            if (fgets(content, BUFFER_SIZE, stdin) == NULL) {
                printf("שגיאה בקריאת קלט, מתנתק...\n");
                break;
            }
            content[strcspn(content, "\n")] = '\0';
        }
        if (strcmp(command, "WRITE") == 0 || strcmp(command, "APPEND") == 0) {
            snprintf(message, sizeof(message), "%s|%s|%s", command, filename, content);
        } else {
            snprintf(message, sizeof(message), "%s|%s", command, filename);
        }

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
            printf("השרת סגר את החיבור\n");
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Server response: %s\n", buffer);
    }
    close(sock_fd);
    printf("החיבור נסגר\n");
    return 0;
}