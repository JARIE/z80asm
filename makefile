# File: makefile
# Created: 27, may 2014

#  Copyright (C) 2014 Jarielle Catbagan
#  
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.


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
