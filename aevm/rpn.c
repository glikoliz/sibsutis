#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
  const char* infixExpression = "(A + B) * A";
  char* postfixExpression = getRPN(infixExpression);

  printf("Infix: %s\n", infixExpression);
  printf("Postfix: %s\n", postfixExpression);

  free(postfixExpression);

  return 0;
}
