#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>
/*
 * A definition for a huffman tree
 */
struct huffmantree{
	int c;
	int count;
	struct huffmantree* left;
	struct huffmantree* right;
};
typedef struct huffmantree huffmantree;
extern huffmantree* huffmantree_init();
extern huffmantree* huffmantree_init_from_string(char* string);
extern char* huffmantree_tostring();
extern int huffmantree_isempty(huffmantree* tree);
extern int huffmantree_isleaf(huffmantree* tree);
extern void huffmantree_free(huffmantree* tree);