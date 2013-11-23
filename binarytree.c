#include "binarytree.h"
binarytree* binarytree_init(int datasize);
huffmantree* huffmantree_init();
huffmantree* huffmantree_init(char* string);
char* huffmantree_tostring();
bst* bst_init(int datasize,int (*cmp)(void* a, void* b));
void bst_add(bst* tree,void* elem);
void bst_isin(bst* tree,void* elem);
void bst_rm(bst* tree, void* elem);
int binarytree_isempty(binarytree* tree);
int binarytree_isleaf(binarytree* tree);
int huffmantree_isempty(huffmantree* tree);
int huffmantree_isleaf(huffmantree* tree);
int bst_isempty(bst* tree);
int bst_isleaf(bst* tree);
void binarytree_free(binarytree* tree);
void huffmantree_free(huffmantree* tree);
void bst_free(bst* tree);