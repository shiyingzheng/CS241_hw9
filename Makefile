all: sortedlist linkedlist sortedlist_test

linkedlist: linkedlist.c
	clang -g -c linkedlist.c

sortedlist: linkedlist.o sortedlist.c
	clang -g -c sortedlist.c

sortedlist_test: sortedlist.c sortedlist.o linkedlist.o
	clang -std=c99 -g -o sortedlist_test sortedlist_test.c sortedlist.o linkedlist.o

huffmantree: huffmantree.c
	clang -g -c huffmantree.c -lm

clean:
	rm -f sortedlist.o linkedlist.o sortedlist_test huffmantree *~
