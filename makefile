all:
	gcc main.c util.c scan.c parse.c -o tiny

modified: main.o util.o scan.o parse.o
		  gcc main.o util.o scan.o parse.o -o tiny

main.o: main.c
		gcc -c main.c
scan.o: scan.c
		gcc -c scan.c
parse.o: parse.c
		gcc -c parse.c
util.o: util.c
		gcc -c util.c

clean:
	rm -rf *.o tiny
