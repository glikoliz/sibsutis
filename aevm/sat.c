#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <string.h>
#include "lib/ui.h"
#include "lib/memory.h"

#define ERROR_ARGS 1
#define ERROR_FILE 2
#define ERROR_OUT 3
#define ERROR_SYNTAX 4
#define ERROR_ADDR 16
#define ERROR_CMD 17
#define ERROR_OPERAND 18
#define ERROR_ENCODE 19
#define ERROR_EOF 20
int isNumeric(const char * str)
{
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }

    return 1;
}
int def_int(FILE *f, int addr)
{
    char operand[10];
    if (!fscanf(f, "%s", operand)) {
        return ERROR_OPERAND;
    }
    int value=atoi(operand);
    sc_memorySet(addr, value);
    return 0;
}

int readLine(FILE *f)
{
    int addr;
    if (feof(f)) {
        return ERROR_EOF;
    }

    if (!fscanf(f, "%d", &addr)) {
        return ERROR_ADDR;
    }

    char command[10] = "\0";
    if (feof(f)) {
        return ERROR_EOF;
    }

    if (!fscanf(f, "%s", command)) {
        return ERROR_CMD;
    }

    int cmd_id = command_id(command);
    if (cmd_id == 0) {
        if(command[0] == '=')
        {
            return def_int(f, addr);
        }
        if (isNumeric(command)) {
            sc_memorySet(addr, atoi(command));
            fscanf(f, " ");
            return 0;
        }
        return ERROR_CMD; 
    }

    int operand;
    if (feof(f)) {
        return ERROR_EOF;
    }

    if (!fscanf(f, "%d", &operand)) {

        return ERROR_OPERAND;
    }

    int value;
    if (sc_commandEncode(cmd_id, operand, &value)) {

        return ERROR_ENCODE;
    }

    sc_memorySet(addr, value);
    fscanf(f, " ");
    return 0;
}

int readFile(FILE *f, const char * filename)
{
    int line = 0;
    int errors = 0;
    while(!feof(f)) {
        line++;
        int result = readLine(f);
        if (!result) {
            continue;
        }

        errors++;

        if (result == ERROR_ADDR || result == ERROR_CMD || result == ERROR_ENCODE) {
            printf("line %d: expected address like \"00 LOAD 01\"\n", line);
        } else if (result == ERROR_ENCODE) {
            printf("line %d: can't encode command\n", line);
        } else if (result == ERROR_EOF) {
            printf("line %d: EOF\n", line);
        }

        break;
    }

    if (sc_memorySave(filename)) {
        printf("can't open file %s\n", filename);
        return ERROR_OUT;
    }

    return 0;
}

int main(int argc, char * argv[])
{
    FILE *f;
    if (argc < 2) {
        printf("Wrong args parsed\nUsage: %s filename.sa filename.o\n", argv[0]);
        return ERROR_ARGS;
    }
    char * filename = argv[1];

    f = fopen(filename, "r");
    if (!f) {
        printf("Wrong args parsed\nUsage: %s filename.sa filename.o\n", argv[0]);
        return ERROR_FILE;
    }

    sc_memoryInit();
    return readFile(f, argv[2]);
}