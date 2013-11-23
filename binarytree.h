#include <stdio.h>
#include <stdlib.h>
/*
 * A definition for a binary tree
 */
typedef struct binarytree{
	int elemsize;
	void* data;
	binarytree* left;
	binarytree* right;
} binarytree;
typedef struct huffmantree{
	char c;
	int count;
	huffmantree* left;
	huffmantree* right;
} huffmantree;
typedef struct{
	binarytree* tree;
	int (*cmp)(void* a, void* b);
} binarysearchtree;
typedef binarysearchtree bst;
binarytree* binarytree_init(int datasize);
huffmantree* huffmantree_init();
huffmantree* huffmantree_init(char* string);
char* huffmantree_tostring();
bst* bst_init(int datasize,int (*cmp)(void* a, void* b));
void bst_add(bst* tree,void* elem);
void bst_isin(bst* tree,void* elem);
void bst_rm(bst* tree, void* elem);