#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

#define BACKLOG_SIZE 5

int create_server_socket(int port);

int accept_client(int server_fd);

#endif