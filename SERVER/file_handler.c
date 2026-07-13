#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

#include "file_handler.h"

#define WORKING_DIR "server_files"
#define MAX_PATH_LEN 512
#define READ_CHUNK_SIZE 900

void ensure_working_dir_exists(void) {
    if (mkdir(WORKING_DIR, 0755) < 0 && errno != EEXIST) {
        perror("mkdir failed");
    }
}

static void build_full_path(const char *filename, char *full_path, size_t size) {
    snprintf(full_path, size, "%s/%s", WORKING_DIR, filename);
}

static int is_existing_regular_file(const char *full_path) {
    struct stat st;
    if (lstat(full_path, &st) < 0) {
        return 0; 
    }
    return S_ISREG(st.st_mode);
}

void handle_create(const char *filename, char *response) {
    char full_path[MAX_PATH_LEN];
    build_full_path(filename, full_path, sizeof(full_path));

    int fd = open(full_path, O_CREAT | O_EXCL | O_WRONLY, 0644);
    if (fd < 0) {
        if (errno == EEXIST) {
            snprintf(response, MAX_RESPONSE_LEN, "ERROR|File already exists");
        } else {
            snprintf(response, MAX_RESPONSE_LEN, "ERROR|Could not create file");
        }
        return;
    }

    close(fd);
    snprintf(response, MAX_RESPONSE_LEN, "OK|File created successfully");
}

void handle_write(const char *filename, const char *content, char *response) {
    char full_path[MAX_PATH_LEN];
    build_full_path(filename, full_path, sizeof(full_path));

    if (!is_existing_regular_file(full_path)) {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|File does not exist");
        return;
    }

    int fd = open(full_path, O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|Could not open file for writing");
        return;
    }

    ssize_t written = write(fd, content, strlen(content));
    close(fd);

    if (written < 0) {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|Failed to write to file");
        return;
    }

    snprintf(response, MAX_RESPONSE_LEN, "OK|File written successfully");
}

void handle_append(const char *filename, const char *content, char *response) {
    char full_path[MAX_PATH_LEN];
    build_full_path(filename, full_path, sizeof(full_path));

    if (!is_existing_regular_file(full_path)) {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|File does not exist");
        return;
    }

    int fd = open(full_path, O_WRONLY | O_APPEND, 0644);
    if (fd < 0) {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|Could not open file for appending");
        return;
    }

    ssize_t written = write(fd, content, strlen(content));
    close(fd);

    if (written < 0) {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|Failed to append to file");
        return;
    }

    snprintf(response, MAX_RESPONSE_LEN, "OK|Content appended successfully");
}

void handle_read(const char *filename, char *response) {
    char full_path[MAX_PATH_LEN];
    build_full_path(filename, full_path, sizeof(full_path));

    if (!is_existing_regular_file(full_path)) {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|File does not exist");
        return;
    }

    int fd = open(full_path, O_RDONLY);
    if (fd < 0) {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|Could not open file for reading");
        return;
    }

    char file_content[READ_CHUNK_SIZE];
    ssize_t bytes_read = read(fd, file_content, sizeof(file_content) - 1);
    close(fd);

    if (bytes_read < 0) {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|Failed to read file");
        return;
    }

    file_content[bytes_read] = '\0';
    snprintf(response, MAX_RESPONSE_LEN, "OK|%s", file_content);
}

void handle_delete(const char *filename, char *response) {
    char full_path[MAX_PATH_LEN];
    build_full_path(filename, full_path, sizeof(full_path));

    if (!is_existing_regular_file(full_path)) {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|File does not exist");
        return;
    }

    if (unlink(full_path) < 0) {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|Failed to delete file");
        return;
    }

    snprintf(response, MAX_RESPONSE_LEN, "OK|File deleted successfully");
}