#include "binarytree.h"
binarytree* binarytree_init(int datasize);
bst* bst_init(int datasize,int (*cmp)(void* a, void* b));
void bst_add(bst* tree,void* elem);
void bst_isin(bst* tree,void* elem);
void bst_rm(bst* tree, void* elem);
int binarytree_isempty(binarytree* tree);
int binarytree_isleaf(binarytree* tree);
int bst_isempty(bst* tree);
int bst_isleaf(bst* tree);
void binarytree_free(binarytree* tree);
void bst_free(bst* tree);