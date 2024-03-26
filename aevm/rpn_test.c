#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *input = NULL, *output = NULL;

int isOperator(char c) {
  return (c == '+' || c == '-' || c == '*' || c == '/');
}

int getPriority(char c) {
  if (c == '+' || c == '-') return 1;
  else if (c == '*' || c == '/') return 2;
  return 0;
}

char* getRPN(const char* infix) {
  int len = strlen(infix);
  char* postfix = (char*)malloc((2 * len + 1) * sizeof(char));
  int postfixIndex = 0;

  char stack[len];
  int stackIndex = 0;

  for (int i = 0; i < len; i++) {
    char currentChar = infix[i];

    if (isOperator(currentChar)) {
      if (postfixIndex > 0 && postfix[postfixIndex - 1] != ' ') {
        postfix[postfixIndex++] = ' ';
      }
      while (stackIndex > 0 && getPriority(stack[stackIndex - 1]) >= getPriority(currentChar)) {
        postfix[postfixIndex++] = stack[--stackIndex];
        postfix[postfixIndex++] = ' ';
      }
      stack[stackIndex++] = currentChar;
    } else if (currentChar == '(') {
      stack[stackIndex++] = currentChar;
    } else if (currentChar == ')') {
      while (stackIndex > 0 && stack[stackIndex - 1] != '(') {
        postfix[postfixIndex++] = ' ';
        postfix[postfixIndex++] = stack[--stackIndex];
      }
      stackIndex--;
    } else {
      postfix[postfixIndex++] = currentChar;
    }
  }

  while (stackIndex > 0) {
    postfix[postfixIndex++] = ' ';
    postfix[postfixIndex++] = stack[--stackIndex];
  }

  postfix[postfixIndex] = '\0';
  return postfix;
}

int assembler_counter = 0;
const int MAX_VARIABLES = 26;
typedef struct variable
{
    char name;
    int address;
    int value;
} variable;

variable variables[52];
int variable_counter = 0;

int isVariable(char name)
{
    if (name >= 'A' && name <= 'Z')
    {
        return 0;
    }
    return 1;
}
int getVariableAddress(char name)
{
    for (int i = 0; i < MAX_VARIABLES * 2;
         i++) // constants are lowercase, variables - uppercase
    {
        if (variables[i].name == name)
        {
            return variables[i].address;
        }
        if (isVariable(variables[i].name) != 0 && !(variables[i].name >= 'a' && variables[i].name <= 'z'))
        {
            variables[i].name = name;
            variables[i].address = 99 - i;
            variable_counter++;
            return variables[i].address;
        }
    }
    return 0;
}

void assemblerOutput(char *cmd, int operand)
{
    printf("%.2i %s %.2d\n", assembler_counter, cmd, operand);
    fprintf(output, "%.2i %s %.2d\n", assembler_counter, cmd, operand);
    assembler_counter++;
}

void parseRPN(char *rpn, char var)
{
    char last_const_alias = 'a';
    char memoryCounter = last_const_alias;
    int depth = 0;
    for (int i = 0; rpn[i] != '\0' && rpn[i] != '\n'; i++)
    {
        if ((rpn[i] >= 'a' && rpn[i] <= 'z') || isVariable(rpn[i]) == 0)
        {
            assemblerOutput("LOAD", getVariableAddress(rpn[i]));
            assemblerOutput("STORE", getVariableAddress(memoryCounter));
            memoryCounter++;
            last_const_alias++;
            depth++;

        }
        else if (rpn[i] == '+' || rpn[i] == '-' || rpn[i] == '*' || rpn[i] == '/')
        {
            if (depth >= 2)
            {
                int operand1 = getVariableAddress(memoryCounter - 2);
                int operand2 = getVariableAddress(memoryCounter - 1);
                assemblerOutput("LOAD", operand1);
                switch (rpn[i])
                {
                case '+':
                    assemblerOutput("ADD", operand2);
                    break;
                case '-':
                    assemblerOutput("SUB", operand2);
                    break;
                case '/':
                    assemblerOutput("DIVIDE", operand2);
                    break;
                case '*':
                    assemblerOutput("MUL", operand2);
                    break;
                }
                assemblerOutput("STORE",
                                getVariableAddress(memoryCounter - 2));
                depth--;
                memoryCounter--;
            }
        }
    }
    assemblerOutput("STORE", getVariableAddress(var));
}
int if_goto(char input[], int *values, int line_number, int rem_count)
{
    char *token = strtok(input, " ");

    if (!token)
        return 1;

    assemblerOutput("LOAD", values[token[0] - 'A']);
    line_number++;
    token = strtok(NULL, " ");

    if (token && (strcmp(token, "<") == 0 || strcmp(token, ">") == 0 || strcmp(token, "=") == 0))
    {
        char* condition = strcmp(token, "<") == 0 ? "JNEG" : (strcmp(token, ">") == 0 ? "JNS" : "JZ");

        token = strtok(NULL, " ");
        if (token && atoi(token) == 0)
        {
            token = strtok(NULL, " ");
            if (token && strcmp(token, "GOTO") == 0)
            {
                token = strtok(NULL, " ");
                if (token)
                {
                    assemblerOutput(condition, atoi(token) - rem_count);
                    return 0;
                }
            }
        }
    }

    return 1;
}
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Wrong args\nUsage: %s basic.sa assembler.sa\n", argv[0]);
    }
    FILE *input_file = fopen(argv[1], "r");
    output=fopen(argv[2], "w");

    char line[1024];
    int rem_count=0;
    int read_memory_cell = 99;
    int values[26] = {0};
    while (fgets(line, sizeof(line), input_file))
    {
        int line_number;
        char operator[16];
        char parameters[256];
        sscanf(line, "%d %s %[^\n]", &line_number, operator, parameters);

        if (strcmp(operator, "INPUT") == 0)
        {
            fprintf(output, "%02d READ %02d\n", assembler_counter, read_memory_cell);
            getVariableAddress(variables[0].name);
            printf("%02d READ %02d\n", assembler_counter, read_memory_cell);
            assembler_counter++;
            values[parameters[0] - 65] = read_memory_cell;
            read_memory_cell-=2;
        }
        else if (strcmp(operator, "LET") == 0)
        {
            char* rpn_str = getRPN(parameters+4);
            parseRPN(rpn_str, parameters[0]);
            values[parameters[0]-65]=getVariableAddress(parameters[0]);
        }
        else if (strcmp(operator, "REM") == 0)
        {
            rem_count++;
        }
        else if (strcmp(operator, "IF") == 0)
        {
            if_goto(parameters, values, assembler_counter, rem_count);
        }
        else if (strcmp(operator, "PRINT") == 0)
        {
            printf("%02d WRITE %02d\n", assembler_counter, values[parameters[0] - 65]);
            fprintf(output, "%02d WRITE %02d\n", assembler_counter, values[parameters[0] - 65]);
            assembler_counter++;
        }
        else if (strcmp(operator, "END") == 0)
        {
            printf("%02d HALT 00\n", assembler_counter);
            fprintf(output, "%02d HALT 00", assembler_counter);
                        assembler_counter++;
        }
    }

    fclose(input_file);
    fclose(output);
    // parseRPN("A B + C * D -", 'Z');
    // for(int i=0; i<26;i++)
    // {
    //     printf("%c %d\n", 'A'+i,getVariableAddress('A'+i));
    // }
}