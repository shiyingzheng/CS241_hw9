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
extern binarytree* binarytree_init(int datasize);
extern huffmantree* huffmantree_init();
extern huffmantree* huffmantree_init(char* string);
extern char* huffmantree_tostring();
extern bst* bst_init(int datasize,int (*cmp)(void* a, void* b));
extern void bst_add(bst* tree,void* elem);
extern void bst_isin(bst* tree,void* elem);
extern void bst_rm(bst* tree, void* elem);
extern int binarytree_isempty(binarytree* tree);
extern int binarytree_isleaf(binarytree* tree);
extern int huffmantree_isempty(huffmantree* tree);
extern int huffmantree_isleaf(huffmantree* tree);
extern int bst_isempty(bst* tree);
extern int bst_isleaf(bst* tree);
extern void binarytree_free(binarytree* tree);
extern void huffmantree_free(huffmantree* tree);
extern void bst_free(bst* tree);