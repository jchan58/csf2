CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c11

OBJS = magic.o elf_names.o

%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $*.o

magic : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

magic.o : magic.c elf_names.h

elf_names.o : elf_names.c elf_names.h

solution.zip : Makefile magic.c elf_names.c elf_names.h README.txt
	zip -9r $@ Makefile magic.c elf_names.c elf_names.h README.txt

clean :
	rm -f *.o magic solution.zip
