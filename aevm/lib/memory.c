#include "memory.h"

sc_reg_t sc_register;
int memory[MEMSIZE];
int sc_memoryInit()
{
    memset(memory, 0, MEMSIZE * sizeof(memory[0]));
    return 0;
}

int sc_memorySet(int address, int value)
{
    if (address < 100 && address >= 0) {
        memory[address] = value;
        return 0;
    }

    sc_regSet(FLAG_MEMORY_ERROR, 1);
    return ERROR_ILLEGAL_ADDRESS;
}

int sc_memoryGet(int address, int * value)
{
    if (address < 100 && address >= 0) {
        *value = memory[address];
        return 0;
    }

    sc_regSet(FLAG_MEMORY_ERROR, 1);
    return ERROR_ILLEGAL_ADDRESS;
}

int sc_memorySave(const char *filename)
{
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        return ERROR_IO_FILE;
    }

    fwrite((void*)memory, sizeof(memory[0]), MEMSIZE, f);
    fclose(f);
    return 0;
}

int sc_memoryLoad(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        return ERROR_IO_FILE;
    }

    fread(memory, sizeof(memory[0]), MEMSIZE, f);
    fclose(f);
    return 0;
}

int sc_regInit()
{
    sc_register.flag = 0;
    return 0;
}

int sc_regSet(int register_id, int value)
{
    if (value != 1 && value != 0) {
        return ERROR_WRONG_REGISTER_VALUE;
    }

    switch (register_id) {
        case FLAG_OVERFLOW:
        case FLAG_DIVISION_BY_ZERO:
        case FLAG_MEMORY_ERROR:
        case FLAG_TICK_IGNORE:
        case FLAG_WRONG_COMMAND:
            if (value) {
                sc_register.flag |= register_id;
            } else {
                sc_register.flag &= ~register_id;
            }
            return 0;
    }

    return ERROR_WRONG_REGISTER_SELECTED;
}

int sc_regGet(int register_id, int *value)
{
    switch (register_id)
    {
        case FLAG_OVERFLOW:
        case FLAG_DIVISION_BY_ZERO:
        case FLAG_MEMORY_ERROR:
        case FLAG_TICK_IGNORE:
        case FLAG_WRONG_COMMAND:
            *value = (sc_register.flag & register_id) ? 1 : 0;
            return 0;
    }

    return ERROR_WRONG_REGISTER_SELECTED;
}

int sc_commandEncode(int command, int operand, int *value)
{
    if (command < 0 || command > 127 || operand < 0 || operand > 127) {
        return ERROR_INVALID_COMMAND;
    }

    *value = (command << 7) | operand;
    return 0;
}

int sc_commandDecode(int value, int *command, int *operand)
{
    if (value < 0 || value > 16383) {
        return ERROR_INVALID_COMMAND;
    }

    *command = (value >> 7) & 0x7F;
    *operand = value & 0x7F;
    return 0;
}
int sc_instGet(int *value)
{
    *value = sc_register.count;
    return 0;
}

int sc_instSet(int value)
{
    if (value >= 100) {
        return -1;
    }

    sc_register.count = value;
    return 0;
}
int sc_accumGet(int *value)
{
    *value = sc_register.accum;
    return 0;
}

int sc_accumSet(int value)
{
/*    if (value > 127 || value < -128)
        return -1;*/
    sc_register.accum = value;
    return 0;
}