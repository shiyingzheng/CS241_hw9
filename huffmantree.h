#include <stdio.h>
#include <stdlib.h>
/*
 * A definition for a huffman tree
 */
typedef struct huffmantree{
	char c;
	int count;
	huffmantree* left;
	huffmantree* right;
} huffmantree;
extern huffmantree* huffmantree_init();
extern huffmantree* huffmantree_init(char* string);
extern char* huffmantree_tostring();
extern int huffmantree_isempty(huffmantree* tree);
extern int huffmantree_isleaf(huffmantree* tree);
extern void huffmantree_free(huffmantree* tree);