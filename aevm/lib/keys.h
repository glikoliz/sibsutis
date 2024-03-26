#ifndef KEYS_H
#define KEYS_H
#include <termios.h>

extern struct termios def;
extern struct termios custom;

extern enum keys {
    up, down, left, right, f5, f6, f7, f9, q, l, s, i, t, r, enter, none
} keys;

int rk_readkey(enum keys *key);
int rk_mytermsave();
int rk_mytermrestore();
int rk_mytermregime(int regime, int vtime, int vmin, int echo, int sigint);
#endif