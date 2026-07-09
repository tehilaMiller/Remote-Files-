#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "parser.h"

#define MAX_ERROR_MSG_LEN 128

int validate_command(const ParsedCommand *parsed, char *error_msg);

#endif
