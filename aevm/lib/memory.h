#ifndef ARCH_LIB_MEMORY_H
#define ARCH_LIB_MEMORY_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ERROR_WRONG_REGISTER_SELECTED 1
#define ERROR_WRONG_REGISTER_VALUE 2
#define ERROR_INVALID_COMMAND 4

typedef enum {
    FLAG_OVERFLOW = 1,
    FLAG_DIVISION_BY_ZERO = 2,
    FLAG_MEMORY_ERROR = 4,
    FLAG_TICK_IGNORE = 8,
    FLAG_WRONG_COMMAND = 16
} sc_flag_t;

typedef struct {
    int accum;
    int count;
    int flag;
} sc_reg_t;
extern sc_reg_t sc_register;


#define MEMSIZE 100
extern int memory[MEMSIZE];

int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int * value);
int sc_memorySave(const char * filename);
int sc_memoryLoad(const char * filename);
int sc_regInit();
int sc_regSet(int register_id, int value);
int sc_regGet(int register_id, int *value);
int sc_commandEncode(int command, int operand, int *value);
int sc_commandDecode(int value, int *command, int *operand);
int sc_accumSet(int value);
int sc_accumGet(int *value);
int sc_instGet(int *value);
int sc_instSet(int value);

#define ERROR_ILLEGAL_ADDRESS 1
#define ERROR_IO_FILE 2

#endif
