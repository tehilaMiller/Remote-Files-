#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
 
#define MAX_RESPONSE_LEN 1024
 
void ensure_working_dir_exists(void);
void handle_create(const char *filename, char *response);
void handle_write(const char *filename, const char *content, char *response);
void handle_append(const char *filename, const char *content, char *response);
void handle_read(const char *filename, char *response);
void handle_delete(const char *filename, char *response);
 
#endif
 