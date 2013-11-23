#include "huffmantree.h"
#define NOCHAR -2;
void error(){
	exit(3);
}
huffmantree* huffmantree_init(){
	huffmantree* tree=malloc(sizeof(huffmantree));
	if(!tree) error();
	tree->c=NOCHAR;
	tree->count=0;
	tree->left=NULL;
	tree->right=NULL;
	return tree;
}
huffmantree* huffmantree_init_from_string(char* string){
	huffmantree* tree=malloc(sizeof(huffmantree));
	if(!tree) error();
	tree->c=string;
	tree->count=0;
	tree->left=NULL;
	tree->right=NULL;
	return tree;
}
char* huffmantree_tostring(huffmantree* tree){

}
int huffmantree_isempty(huffmantree* tree){
	if(!tree->count) return 1;
	return 0;
}
int huffmantree_isleaf(huffmantree* tree){
	if(!tree->left&&!tree->right) return 1;
	return 0;
}
void huffmantree_free(huffmantree* tree){
	if (tree->right){
		huffmantree_free(tree->right);
		free(tree);
	}
	if (tree->left){
		huffmantree_free(tree->left);
		free(tree);
	}
	free(tree);
}