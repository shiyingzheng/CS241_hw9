all: sortedlist.o linkedlist.o sortedlist_test huffmantree.o huffmantree_test encode decode

linkedlist.o: linkedlist.c
	clang -g -c linkedlist.c

sortedlist.o: linkedlist.o sortedlist.c
	clang -g -c sortedlist.c

huffmantree.o: huffmantree.c sortedlist.o
	clang -g -c huffmantree.c

sortedlist_test: sortedlist.c sortedlist.o linkedlist.o
	clang -std=c99 -g -o sortedlist_test sortedlist_test.c sortedlist.o linkedlist.o -lm

huffmantree_test: huffmantree.c huffmantree.o sortedlist.o linkedlist.o
	clang -std=c99 -g -o huffmantree_test huffmantree_test.c huffmantree.o sortedlist.o linkedlist.o -lm

encode: encode.c huffmantree.o sortedlist.o linkedlist.o
	clang -std=c99 -g -o encode encode.c huffmantree.o sortedlist.o linkedlist.o -lm

decode: decode.c huffmantree.o sortedlist.o linkedlist.o
	clang -std=c99 -g -o decode decode.c huffmantree.o sortedlist.o linkedlist.o

clean:
	rm -f sortedlist.o linkedlist.o sortedlist_test huffmantree.o huffmantree_test encode decode *~
