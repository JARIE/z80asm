# File: makefile
# Author: Jarielle Catbagan
# Created: 27, may 2014

TARGET = z80asm
DEPENDENCIES = z80asm.o udgetopt.o parse.o task.o assemble.o
CC = gcc

build: $(DEPENDENCIES)
	$(CC) -o $(TARGET) $(DEPENDENCIES)
z80asm.o: z80asm.c udgetopt.h defines.h parse.h task.h assemble.h z80instructionset.h
	$(CC) -c z80asm.c
udgetopt.o: udgetopt.c
	$(CC) -c udgetopt.c
parse.o: parse.c defines.h parse.h
	$(CC) -c parse.c
task.o: task.c defines.h
	$(CC) -c task.c
assemble.o: assemble.c defines.h assemble.h
	$(CC) -c assemble.c
clean:
	rm -f $(TARGET).exe $(TARGET).exe.stackdump $(DEPENDENCIES)
