#include <string.h>
#include "validator.h"

int is_known_command(const char *command) {
    return (strcmp(command, "CREATE") == 0 ||
            strcmp(command, "WRITE") == 0 ||
            strcmp(command, "APPEND") == 0 ||
            strcmp(command, "READ") == 0 ||
            strcmp(command, "DELETE") == 0 ||
            strcmp(command, "QUIT") == 0);
}

int is_valid_filename(const char *filename) {
    if(strlen(filename) == 0 ){
        return 0; 
    }
    if(strstr(filename, "..") != NULL ) {
        return 0; 
    }
    if(filename[0] == '/') {
        return 0; 
    }
    return 1; 
}