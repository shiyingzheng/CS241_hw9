all: sortedlist linkedlist sortedlist_test huffmantree huffmantree_test

linkedlist: linkedlist.c
	clang -g -c linkedlist.c

sortedlist: linkedlist.o sortedlist.c
	clang -g -c sortedlist.c

huffmantree: huffmantree.c sortedlist.o
	clang -g -c huffmantree.c

sortedlist_test: sortedlist.c sortedlist.o linkedlist.o
	clang -std=c99 -g -o sortedlist_test sortedlist_test.c -lm sortedlist.o linkedlist.o

huffmantree_test: huffmantree.c huffmantree.o
	clang -std=c99 -g -o huffmantree_test huffmantree_test.c huffmantree.o sortedlist.o linkedlist.o -lm

clean:
	rm -f sortedlist.o linkedlist.o sortedlist_test huffmantree.o huffmantree_test *~
