#ifndef ARCH_LIB_COMMAND_H
#define ARCH_LIB_COMMAND_H
#include "memory.h"

// #define COMMAND_ERROR 1
int command_id(char * command_name);
const char * command_name(int command) ;

#endif