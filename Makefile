.PHONY: default compile run

CC = gcc
flags = -Wall -g

default: compile run

compile:
	$(CC) $(flags) -c probe.c -o probe.o
	$(CC) $(flags) -c stack.c -o stack.o
	$(CC) $(flags) stack.o probe.o main.c -o main

run:
	./main -t .

clean:
	rm -f *.o
