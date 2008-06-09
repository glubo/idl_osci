all: mainwindow file.o
mainwindow: mainwindow.c file.o
	gcc -o mainwindow  mainwindow.c file.o -Wall `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`

file.o: file.h file.c dprintf.h
	gcc -c file.c
