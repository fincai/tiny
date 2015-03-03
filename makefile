all:
	gcc main.c util.c scan.c parse.c symtab.c analyze.c -o tiny
pbison:
	gcc -ansi main.c util.c scan.c parser.tab.c symtab.c analyze.c -o tiny

modified: main.o util.o scan.o parse.o symtab.o analyze.o
		  gcc main.o util.o scan.o parse.o analyze.o symtab.o -o tiny

main.o: main.c
		gcc -c main.c
scan.o: scan.c
		gcc -c scan.c
parse.o: parse.c
		gcc -c parse.c
symtab.o: symtab.c
		gcc -c symtab.c
analyze.o:
		gcc -c analyze.c
util.o: util.c
		gcc -c util.c

clean:
	rm -rf *.o tiny
