#include <stdio.h>
#include <string.h>
 
#include "validator.h"

static int is_safe_filename(const char *filename) {
    if (strlen(filename) == 0) {
        return 0; 
    }
    if (strstr(filename, "..") != NULL) {
        return 0; 
    }
    if (filename[0] == '/') {
        return 0; 
    }
    return 1; 
}

int validate_command(const ParsedCommand *parsed, char *error_msg) {
    int known_command=
        (strcmp(parsed->command, "CREATE") == 0 ||
         strcmp(parsed->command, "WRITE") == 0 ||
         strcmp(parsed->command, "APPEND") == 0 ||
         strcmp(parsed->command, "READ") == 0 ||
         strcmp(parsed->command, "DELETE") == 0 ||
         strcmp(parsed->command, "QUIT") == 0);

    if (!known_command) {
        snprintf(error_msg, MAX_ERROR_MSG_LEN, "Unknown command: %s", parsed->command);
        return 0;
    }

    if (strcmp(parsed->command, "QUIT") == 0) {
        return 1;
    }

    if (!is_safe_filename(parsed->filename)) {
        snprintf(error_msg, MAX_ERROR_MSG_LEN, "Invalid or unsafe filename");
        return 0;
    }

    return 1;
}