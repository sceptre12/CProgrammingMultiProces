CC = gcc
CFLAG = -Wall -g

all: build bin

build: main.o helper.o execute.o executefunctions.o
	$(CC) $(CFLAG) -o myshell main.o helperfunctions.o execute.o executefunctions.o

main.o: main.c header.h
	$(CC) $(CFLAG) -c main.c

helper.o: helperfunctions.c header.h
	$(CC) $(CFLAG) -c helperfunctions.c

execute.o: execute.c header.h
	$(CC) $(CFLAG) -c execute.c

executefunctions.o: executefunctions.c header.h
	$(CC) $(CFLAG) -c executefunctions.c

bin:
	mkdir bin
	mv *.o bin/

clean:
	rm -rf bin *.o
