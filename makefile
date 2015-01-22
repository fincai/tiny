all:
	gcc main.c util.c scan.c -o tiny

modified: main.o util.o scan.o
		  gcc main.o util.o scan.o -o tiny

main.o: main.c
		gcc -c main.c
scan.o: scan.c
		gcc -c scan.c
util.o: util.c
		gcc -c util.c

clean:
	rm -rf *.o tiny
