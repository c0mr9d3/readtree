.PHONY: default compile run

CC = gcc
flags = -Wall -O2

default: compile run

compile:
	$(CC) $(flags) readtree.c -o readtree

run:
	./readtree .
