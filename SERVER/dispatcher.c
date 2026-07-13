#include <stdio.h>
#include <string.h>
 
#include "dispatcher.h"

void dispatch_command(const ParsedCommand *parsed, char *response) {
    if (strcmp(parsed->command, "CREATE") == 0) {
        handle_create(parsed->filename, response);
    } else if (strcmp(parsed->command, "WRITE") == 0) {
        handle_write(parsed->filename, parsed->content, response);
    } else if (strcmp(parsed->command, "APPEND") == 0) {
        handle_append(parsed->filename, parsed->content, response);
    } else if (strcmp(parsed->command, "READ") == 0) {
        handle_read(parsed->filename, response);
    } else if (strcmp(parsed->command, "DELETE") == 0) {
        handle_delete(parsed->filename, response);
    } else {
        snprintf(response, MAX_RESPONSE_LEN, "ERROR|Unsupported command");
    }
}
 