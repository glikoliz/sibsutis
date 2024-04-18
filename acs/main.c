#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handle_signal(int signum) {
    printf("Сигнал %d\n", signum);
    exit(signum);
}

int main(int argc, char *argv[]) {
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    char *env_variable = getenv("EXAMPLE_ENV_VARIABLE");
    if (env_variable != NULL) {
        printf("Переменная окружения: %s\n", env_variable);
    }
    while (1) {}
    
    return 0;
}
