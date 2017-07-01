# Makefile for dijkstra-trolley

CFLAGS = -std=gnu++11 -Wall -O2

all: main

debug: CFLAGS += -DDEBUG
debug: main

main: Node.o main.cpp helpers.cpp
	g++ -o main main.cpp Node.o $(CFLAGS)

Node.o: Node.h Node.cpp
	g++ -c Node.cpp $(CFLAGS)

clean:
	rm -rf *.o *.exe*
