
all: main
main:
	gcc -Wall -g -o graph src/main.c src/graph.h src/graph.c
clean:
	rm -rf graph
