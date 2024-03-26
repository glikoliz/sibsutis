CC = gcc
CFLAGS = -std=gnu11 -Wall -Werror
LIB_FILES = lib/memory.h lib/commands.h lib/units.h lib/term.h lib/big_chars.h lib/chars.h lib/keys.h lib/ui.h
REAL_FILES = lib/memory.c lib/commands.c lib/units.c lib/term.c lib/big_chars.c lib/chars.c lib/keys.c lib/ui.c
OBJ_FILES = lib/memory.o lib/commands.o lib/units.o lib/term.o lib/big_chars.o lib/chars.o lib/keys.o lib/ui.o

.PHONY: all clean lib

all: arch sat sbt

lib: libmysimplelib.a

libmysimplelib.a: $(OBJ_FILES)
		ar rcs $@ $^

arch: arch.c $(LIB_FILES) libmysimplelib.a
		$(CC) $(CFLAGS) $< -o $@ -L. -lmysimplelib

sat: sat.c $(LIB_FILES) libmysimplelib.a
		$(CC) $(CFLAGS) $< -o $@ -L. -lmysimplelib

sbt: sbt.c $(LIB_FILES) libmysimplelib.a
		$(CC) $(CFLAGS) $< -o $@ -L. -lmysimplelib

lib/memory.o: lib/memory.c lib/memory.h
		$(CC) $(CFLAGS) -c $< -o $@

lib/commands.o: lib/commands.c lib/commands.h
		$(CC) $(CFLAGS) -c $< -o $@

lib/units.o: lib/units.c lib/units.h
		$(CC) $(CFLAGS) -c $< -o $@

lib/term.o: lib/term.c lib/term.h
		$(CC) $(CFLAGS) -c $< -o $@

lib/big_chars.o: lib/big_chars.c lib/big_chars.h
		$(CC) $(CFLAGS) -c $< -o $@

lib/chars.o: lib/chars.c lib/chars.h
		$(CC) $(CFLAGS) -c $< -o $@

lib/keys.o: lib/keys.c lib/keys.h
		$(CC) $(CFLAGS) -c $< -o $@

lib/ui.o: lib/ui.c lib/ui.h
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJ_FILES) arch sat sbt libmysimplelib.a
