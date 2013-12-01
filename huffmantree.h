#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include "sortedlist.h"
/*
 * A definition for a huffman tree
 */
struct huffmantree{
	int c;
	int count;
	int size;
	struct huffmantree* left;
	struct huffmantree* right;
};
typedef struct huffmantree huffmantree;
extern void error(void);
extern huffmantree* huffmantree_init();
extern huffmantree* huffmantree_init_from_list_iter(iterator* iter);
extern huffmantree* huffmantree_init_from_stream(FILE* stream);
extern char* huffmantree_tostring();
extern unsigned char* huffmantree_tobits(huffmantree* tree);
extern int huffmantree_isempty(huffmantree* tree);
extern int huffmantree_isleaf(huffmantree* tree);
extern void huffmantree_free(huffmantree* tree);