#include "ui.h"
int coordX, coordY;
int* (*char_func[16])(void) = {char_0, char_1, char_2, char_3, char_4, char_5, char_6, char_7, char_8, char_9, char_a, char_b, char_c, char_d, char_e, char_f};
void print_mem(int x, int y)
{
    int instruction;
    sc_instGet(&instruction);
    for (int i = 0; i < MEMSIZE; i++) {
        int command, operand;
        int value;
        sc_memoryGet(i, &value);
        sc_commandDecode(value, &command, &operand);

        char result[6];
        if (command < 16 && operand < 16) {
            sprintf(result, "0%X:0%X", command, operand);
        } else if (command < 16 && operand >= 16) {
            sprintf(result, "0%X:%X", command, operand);
        } else if (command >= 16 && operand < 16) {
            sprintf(result, "%X:0%X", command, operand);
        } else {
            sprintf(result, "%X:%X", command, operand);
        }

        if (i % 10 == y && i / 10 == x) {
            mt_setbgcolor(COLOR_RED);
            mt_gotoXY(0, 23);
            mt_resetcolor();
        }

        mt_gotoXY(2 + 6 * (i % 10), 2 + (i / 10));
        if (i == instruction) {
            mt_setbgcolor(COLOR_RED);
        }
        if (i % 10 == y && i / 10 == x) {
            mt_setbgcolor(COLOR_GREEN);
        }

        write(1, result, strlen(result));
        mt_resetcolor();
    }
}

void print_accum()
{
    mt_gotoXY(71, 2);
    int acc = 0;
    sc_accumGet(&acc);
    char tmp[5];
    sprintf(tmp, "%4d", acc);
    print_on_screen(tmp);
}

void print_instcnt()
{
    mt_gotoXY(63, 5);
    int inst = 0;
    sc_instGet(&inst);
    int memory_contents, command, operand;
    sc_memoryGet(inst, &memory_contents);
    sc_commandDecode(memory_contents, &command, &operand);
    char tmp[6];
    sprintf(tmp, "%d", inst);
    print_on_screen(tmp);
}

void print_operation(int x, int y)
{
    int memory_contents;
    sc_memoryGet(x * 10 + y , &memory_contents);
    int command;
    int operation;
    sc_commandDecode(memory_contents, &command, &operation);
    mt_gotoXY(63, 8);
    char tmp[6];
    sprintf(tmp, "%02d:%02d", command, operation);
    print_on_screen(tmp);
}

void print_flags()
{
    char flags[] = "ODMTC";
    mt_gotoXY(67, 11);
    for (int i = 0; i < 5; i++) {
        int value;
        sc_regGet(1 << i, &value);

        if (value) {
            mt_setfgcolor(COLOR_RED);
        } else {
            mt_setfgcolor(COLOR_WHITE);
        }
        char tmp[6];
        sprintf(tmp, "%c", flags[i]);
        print_on_screen(tmp);
        mt_resetcolor();
    }
}

void print_bigmemory(int x, int y)
{
    bc_box(1, 13, 50, 10);
    bc_printbigchar(char_plus(), 2, 14, COLOR_WHITE, COLOR_BLACK);
    int num;
    sc_instGet(&num);
    int value;
    sc_memoryGet(num, &value);
    bc_printbigchar(char_func[value/1000](), 2 + 10 * (1), 14, COLOR_WHITE, COLOR_BLACK);
    bc_printbigchar(char_func[value/100%10](), 2 + 10 * (2), 14, COLOR_WHITE, COLOR_BLACK);
    bc_printbigchar(char_func[value/10%10](), 2 + 10 * (3), 14, COLOR_WHITE, COLOR_BLACK);
    bc_printbigchar(char_func[value%10](), 2 + 10 * (4), 14, COLOR_WHITE, COLOR_BLACK);
}

void print_keys()
{
    bc_box(51, 13, 33, 10);
    mt_gotoXY(52, 13);
    const char tmp[] = " Keys: ";
    print_on_screen(tmp);
    const char *tmp1[] = {"l - load", "s - save", "r - run", "t - step",
        "i - reset", "F5 - accumulator", "F6 - instructionCounter", "F9 - enter command"};
    for (int i = 0; i < 8; i++) {
        mt_gotoXY(52, 14 + i);
        print_on_screen(tmp1[i]);
    }
}

void load_mem()
{
    bc_box(20, 6, 20, 5);
    mt_gotoXY(24, 7);
    print_on_screen("Load\n");
    char tmp[255] = "\0";
    mt_gotoXY(21, 9);
    read(1, tmp, 255);
    tmp[strlen(tmp) - 1] = '\0';
    if (sc_memoryLoad(tmp)) {
        bc_box(20, 6, 20, 5);
        mt_gotoXY(23, 7);
        print_on_screen("Failed to open\n");
        mt_gotoXY(29, 9);
        mt_setbgcolor(COLOR_RED);
        print_on_screen("OK");
        mt_resetcolor();
        mt_gotoXY(30, 9);
        read(1, tmp, 1);
    }
    sc_accumSet(0);
    sc_instSet(0);
    refresh(coordX, coordY);
}

void save_mem()
{
    bc_box(20, 6, 20, 5);
    mt_gotoXY(23, 7);
    print_on_screen("Save to\n");
    char tmp[255] = "\0";
    mt_gotoXY(21, 9);
    read(1, tmp, 255);
    tmp[strlen(tmp) - 1] = '\0';
    if (sc_memorySave(tmp)) {
        bc_box(20, 6, 20, 5);
        mt_gotoXY(23, 7);
        print_on_screen("Failed to save\n");
        mt_gotoXY(29, 9);
        mt_setbgcolor(COLOR_RED);
        print_on_screen("OK");
        mt_setbgcolor(COLOR_DEFAULT);
        mt_gotoXY(30, 9);
        read(1, tmp, 1);
    }
    mt_gotoXY(0, 27);
    print_on_screen(tmp);
    refresh(coordX, coordY);
}

void set_accum()
{
    bc_box(20, 6, 20, 5);
    mt_gotoXY(23, 7);
    print_on_screen("Set accum to\n");
    char tmp[5] = "\0";
    mt_gotoXY(21, 9);
    read(1, tmp, 5);
    int new_acc = atoi(tmp);
    sc_accumSet(new_acc);
    refresh(coordX, coordY);
}

void set_instruction()
{
    bc_box(20, 6, 26, 5);
    mt_gotoXY(22, 7);
    print_on_screen("Set instructionCounter to\n");
    char tmp[5];
    mt_gotoXY(21, 9);
    read(1, tmp, 5);
    tmp[strlen(tmp) - 1] = '\0';
    int new_inst = atoi(tmp);
    mt_gotoXY(0, 28);
    sc_instSet(new_inst);
    refresh(coordX,coordY);
}

void enter_command(int command)
{
    char tmp[10];

    if (command == -1) {
        bc_box(20, 6, 26, 5);
        mt_gotoXY(22, 7);
        print_on_screen("Enter command id\n");
        mt_gotoXY(21, 9);
        read(1, tmp, 5);
        tmp[strlen(tmp) - 1] = '\0';

        command = atoi(tmp);
    }
    bc_box(20, 6, 26, 5);
    mt_gotoXY(22, 7);
    print_on_screen("Enter operand\n");
    mt_gotoXY(21, 9);
    read(1, tmp, 5);
    tmp[strlen(tmp) - 1] = '\0';
    int8_t operand = atoi(tmp);

    bc_box(20, 6, 20, 5);
    mt_gotoXY(23, 7);

    int output;
    sc_commandEncode(command, operand, &output);
    sc_memorySet(coordY + coordX*10, output);
    mt_clrscr();

    refresh(coordX,coordY);
}

void enter_command_name()
{
    bc_box(20, 6, 26, 5);
    mt_gotoXY(22, 7);
    print_on_screen("Enter command name\n");
    char tmp[8];
    mt_gotoXY(21, 9);
    read(1, tmp, 7);
    tmp[strlen(tmp) - 1] = '\0';
    int command = atoi(tmp);
    enter_command(command);
}


void set_mem()
{
    bc_box(20, 6, 26, 5);
    mt_gotoXY(22, 7);
    print_on_screen("Set memory(dec) to\n");
    char tmp[11] = "\0";
    mt_gotoXY(21, 9);
    read(1, tmp, 10);
    tmp[strlen(tmp) - 1] = '\0';

    if (tmp[0] != '\n' && tmp[0] != '\0') {
        int tmp1 = atoi(tmp);
        sc_memorySet(coordY + 10*coordX, tmp1);
    }
    refresh(coordX, coordY);
}

void step(int * x, int * y)
{
    CU();

    int current_instruction;
    sc_instGet(&current_instruction);
    *x = current_instruction / 10;
    *y = current_instruction % 10;
}

void init()
{
    sc_regInit();
    sc_regSet(FLAG_OVERFLOW, 0);
    sc_regSet(FLAG_DIVISION_BY_ZERO, 0);
    sc_regSet(FLAG_MEMORY_ERROR, 0);
    sc_regSet(FLAG_WRONG_COMMAND, 0);
    sc_regSet(FLAG_TICK_IGNORE, 1);
    sc_memoryInit();
    sc_instSet(0);
    sc_accumSet(0);
}

void refresh(int x, int y)
{
    bc_box(1, 1, 61, 12);
    mt_gotoXY(30, 0);
    char memory[] = " Memory ";
    print_on_screen(memory);

    bc_box(62, 1, 22, 3);
    mt_gotoXY(66, 1);
    char accumulator[] = " accumulator ";
    print_on_screen(accumulator);

    bc_box(62, 4, 22, 3);
    mt_gotoXY(63, 4);
    char instruction[] = " instructionCounter ";
    print_on_screen(instruction);

    bc_box(62, 7, 22, 3);
    mt_gotoXY(68, 7);
    char operation[] = " Operation ";
    print_on_screen(operation);

    bc_box(62, 10, 22, 3);
    mt_gotoXY(69, 10);
    char flags[] = " Flags ";
    print_on_screen(flags);

    bc_box(1, 13, 50, 10);
    print_keys();
    interface(x, y);
}

void key_handler(int *exit, int * x, int * y)
{
    enum keys key = none;
    rk_readkey(&key);

    int tick_ignore;
    sc_regGet(FLAG_TICK_IGNORE, &tick_ignore);

    if (key == r) {
        tick_ignore = (tick_ignore) ? 0 : 1;
        sc_regSet(FLAG_TICK_IGNORE, tick_ignore);
    } else if (key == up) {
        *x -= 1;

        if (*x < 0) {
            *x = 9;
        }
    } else if (key == down) {
        *x += 1;

        if (*x > 9) {
            *x = 0;
        }
    } else if (key == left) {
        *y -= 1;

        if (*y < 0) {
            *y = 9;
        }
    } else if (key == right) {
        *y += 1;
        if (*y > 9) {
            *y = 0;
        }
    }

    coordX = *x;
    coordY = *y;


    if (!tick_ignore) {
        return;
    }

    if (key == q) {
        sc_regSet(FLAG_TICK_IGNORE, 1);
        *exit = 1;
    }

    if (key == l) {
        load_mem();
    } else if (key == s) {
        save_mem();
    } else if (key == f5) {
        set_accum();
    } else if (key == f6) {
        set_instruction(x, y);
    } else if (key == f7) {
        enter_command(-1);
    } else if (key == f9) {
        enter_command_name();
    } else if (key == t) {
        rk_mytermregime(1, 0, 1, 1, 1);
        step(x, y);
        rk_mytermrestore();
        sc_regSet(FLAG_TICK_IGNORE, 1);
    } else if (key == enter) {
        set_mem();
    } else if (key == i) {
        init();
        sc_regSet(FLAG_TICK_IGNORE, 1);
        *x = 0, *y = 0;
        refresh(*x, *y);
    }


    coordX = *x;
    coordY = *y;
}

int ui_numeric_input(const char * message)
{
    rk_mytermrestore();
    int tick_ignore;
    sc_regGet(FLAG_TICK_IGNORE, &tick_ignore);

    bc_box(20, 6, 20, 5);
    mt_gotoXY(21, 7);
    print_on_screen(message);
    char string_value[10] = "\0";

    mt_gotoXY(21, 9);
    read(1, string_value, 10);

    int int_value = atoi(string_value);

    refresh(coordX, coordY);
    sc_regSet(FLAG_TICK_IGNORE, tick_ignore);
    rk_mytermsave();

    return int_value;
}

char * ui_string_input(const char * message)
{
    rk_mytermrestore();
    int tick_ignore;
    sc_regGet(FLAG_TICK_IGNORE, &tick_ignore);

    bc_box(20, 6, 20, 5);
    mt_gotoXY(21, 7);
    print_on_screen(message);
    char* string_value = (char*)malloc(255);

    mt_gotoXY(21, 9);
    read(1, string_value, 255);

    refresh(coordX, coordY);
    sc_regSet(FLAG_TICK_IGNORE, tick_ignore);
    rk_mytermsave();

    return string_value;
}

void ui_cpu_info(const char * message)
{
    rk_mytermrestore();
    int tick_ignore;
    sc_regGet(FLAG_TICK_IGNORE, &tick_ignore);

    bc_box(20, 6, 35, 3);
    mt_gotoXY(21, 7);
    print_on_screen(message);

    read(1, stdin, 255);

    refresh(coordX, coordY);
    sc_regSet(FLAG_TICK_IGNORE, tick_ignore);
    rk_mytermsave();

}

int ui_readvalue(int address)
{
    rk_mytermrestore();
    int tick_ignore;
    sc_regGet(FLAG_TICK_IGNORE, &tick_ignore);
    sc_regSet(FLAG_TICK_IGNORE, 1);

    bc_box(20, 6, 20, 5);
    mt_gotoXY(21, 7);
    print_on_screen("Input value (dec)\n");
    char string_value[10] = "\0";

    mt_gotoXY(21, 9);
    read(1, string_value, 10);

    int int_value = atoi(string_value);
    sc_memorySet(address, int_value);

    refresh(coordX, coordY);
    sc_regSet(FLAG_TICK_IGNORE, tick_ignore);
    rk_mytermsave();

    return 0;
}

int ui_printvalue(int address)
{
    int tick_ignore;
    sc_regGet(FLAG_TICK_IGNORE, &tick_ignore);
    sc_regSet(FLAG_TICK_IGNORE, 1);

    bc_box(20, 6, 20, 5);
    mt_gotoXY(23, 7);

    int value;
    sc_memoryGet(address, &value);
    char string_value[10] = "\0";
    sprintf(string_value, "%d", value);
    print_on_screen(string_value);


    mt_gotoXY(29, 9);
    mt_setbgcolor(COLOR_RED);
    print_on_screen("OK");

    mt_resetcolor();
    mt_gotoXY(30, 9);
    char buffer[10];
    read(1, buffer, 1);

    sc_regSet(FLAG_TICK_IGNORE, tick_ignore);
    return 0;
}

void interface(int x, int y)
{
    print_mem(x, y);
    print_accum();
    print_instcnt();
    print_operation(x, y);
    print_flags();
    print_bigmemory(x, y);
}

void main_loop()
{
    int exit = 0;
    int cnt = 0;
    int x = 0, y = 0;
            mt_clrscr();

    refresh(x, y);
    while (!exit) {
        interface(x, y);
        key_handler(&exit, &x, &y);
        if (cnt == 15) {
            mt_clrscr();
            refresh(x, y);
            cnt = 0;
        }
        cnt++;
    }
}