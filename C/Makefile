all: mainwindow file.o dir.o conversion.o
DEBUG=-g
set_debug: clean
	sed 's/debug=0/debug=1/' -i dprintf.h 
	sed 's/^DEBUG=$$/DEBUG=-g/' -i Makefile 
unset_debug: clean
	sed 's/debug=1/debug=0/' -i dprintf.h 
	sed 's/^DEBUG=-g/DEBUG=/' -i Makefile 
clean:
	- rm mainwindow
	- rm *.o

mainwindow: mainwindow.c file.o dir.o conversion.o
	gcc -o mainwindow  mainwindow.c file.o dir.o conversion.o -Wall `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` -lm $(DEBUG)

file.o: file.h file.c dprintf.h
	gcc -c file.c `pkg-config --cflags glib-2.0` $(DEBUG) -Wall

conversion.o: conversion.h conversion.c dprintf.h
	gcc -c conversion.c  $(DEBUG) -Wall

dir.o: dir.h dir.c 
	gcc -c dir.c `pkg-config --cflags glib-2.0` $(DEBUG) -Wall
