
CC=gcc -g -Wall 

OBJ= src/vico-serial.o src/vico-funcs.o src/vico.o src/vico-tasks.o src/vico-var.o 
OBJC=vico-serial.c vico-funcs.c vico-tasks.c vico-var.c vico.c

ARDUINODEST=~/sketchbook/libraries/vico/
DEST=/usr/lib
INCLDEST=/usr/include

%.o: %.c
	$(CC) -c -o $@ $< $(OFLAGS)

libvico.a: $(OBJ)
	ar rcs libvico.a $(OBJ)
	
example: example.c libvico.a
	$(CC) example.c -o example -L. -lvico

arduino: $(OBJ)
	rm src/*.o
	cp src/* arduino/vico/src/
	cp -R arduino/vico/* $(ARDUINODEST)

install: libvico.a
	install libvico.a $(DEST)/libvico.a
	install src/vico.h $(INCLDEST)/vico.h

clear:
	rm src/*.o
	rm example
	rm libvico.a
