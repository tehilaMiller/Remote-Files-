#ifndef DISPATCHER_H
#define DISPATCHER_H
 
#include "parser.h"
#include "file_handler.h"

void dispatch_command(const ParsedCommand *parsed, char *response);
 
#endif