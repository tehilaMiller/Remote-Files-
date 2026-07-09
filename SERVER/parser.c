#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

static int is_all_digits(const char *str, size_t len) {
    if (len == 0) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

int parse_command(const char *message, ParsedCommand *result) {
    memset(result, 0, sizeof(ParsedCommand));
    const char *pipe1=strchr(message,'|');

    if(pipe1==NULL){
        size_t len = strlen(message);
        if (len == 0 || len >= MAX_COMMAND_LEN) {
            return 0; 
        }
        strncpy(result->command,message,len);
        result->command[len] = '\0';
        return 1;
    }
    size_t command_len = (size_t)(pipe1 - message);
    if (command_len == 0 || command_len >= MAX_COMMAND_LEN) {
        return 0; 
    }
    strncpy(result->command, message, command_len);
    result->command[command_len] = '\0';
 
    const char *pipe2 = strchr(pipe1 + 1, '|');
    if (pipe2 == NULL) {
        return 0; 
    }
 
    size_t filename_len_digits = (size_t)(pipe2 - (pipe1 + 1));
    char filename_len_str[16];
    if (filename_len_digits == 0 || filename_len_digits >= sizeof(filename_len_str)) {
        return 0;
    }
    if (!is_all_digits(pipe1 + 1, filename_len_digits)) {
        return 0; 
    }
    strncpy(filename_len_str, pipe1 + 1, filename_len_digits);
    filename_len_str[filename_len_digits] = '\0';
 
    long filename_len = atol(filename_len_str);
    if (filename_len < 0 || (size_t)filename_len >= MAX_FILENAME_LEN) {
        return 0;
    }
 
    const char *filename_start = pipe2 + 1;
    if (strlen(filename_start) < (size_t)filename_len) {
        return 0; 
    }
    memcpy(result->filename, filename_start, (size_t)filename_len);
    result->filename[filename_len] = '\0';
 
    const char *pipe3 = filename_start + filename_len;
    if (*pipe3 != '|') {
        return 0; 
    }
 
    const char *pipe4 = strchr(pipe3 + 1, '|');
    if (pipe4 == NULL) {
        return 0; 
    }
 
    size_t content_len_digits = (size_t)(pipe4 - (pipe3 + 1));
    char content_len_str[16];
    if (content_len_digits == 0 || content_len_digits >= sizeof(content_len_str)) {
        return 0;
    }
    if (!is_all_digits(pipe3 + 1, content_len_digits)) {
        return 0;
    }
    strncpy(content_len_str, pipe3 + 1, content_len_digits);
    content_len_str[content_len_digits] = '\0';
 
    long content_len = atol(content_len_str);
    if (content_len < 0 || (size_t)content_len >= MAX_CONTENT_LEN) {
        return 0;
    }
 
    const char *content_start = pipe4 + 1;
    if (strlen(content_start) < (size_t)content_len) {
        return 0; 
    }
    memcpy(result->content, content_start, (size_t)content_len);
    result->content[content_len] = '\0';
 
    return 1;
}