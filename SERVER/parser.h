#ifndef PARSER_H
#define PARSER_H

#define MAX_COMMAND_LEN  16
#define MAX_FILENAME_LEN 256
#define MAX_CONTENT_LEN  1024

typedef struct {
    char command[MAX_COMMAND_LEN];
    char filename[MAX_FILENAME_LEN];
    char content[MAX_CONTENT_LEN];
} ParsedCommand;

int parse_command(const char *message,ParsedCommand *result);

#endif
 