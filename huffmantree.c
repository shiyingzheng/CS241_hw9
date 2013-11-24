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
int to_char(char* string){
	int i;
	int sum=0;
	int k=0;
	for(i=strlen(string)-1;i>=0;i--){
		sum=sum+(string[i]-'0')*(int)pow(2,k);
		k++;
	}
	return sum;
}
huffmantree* huffmantree_init_from_string(char* string){
	huffmantree* tree=malloc(sizeof(huffmantree));
	if(!tree) error();
	tree->c=to_char(string);
	tree->count=0;
	tree->left=NULL;
	tree->right=NULL;
	return tree;
}
char* tobinary(int c){
	int i;
    int mask=1;
    char* val=malloc(sizeof(char)*CHAR_BIT);
	if (c==EOF){
		for(i=0;i<CHAR_BIT;i++){
			val[i]='1';
		}
	}
	else{
		for (i=CHAR_BIT-1;i>=0;--i){
			if (mask&c) val[i]='1';
			else val[i]='0';
			c=c>>1;
		}
	}
	return val;
}
char* huffmantree_tostring(huffmantree* tree){
	if (huffmantree_isleaf(tree)) return strcat("1",tobinary(tree->c));
	else return strcat("0",strcat(huffmantree_tostring(tree->left), huffmantree_tostring(tree->right)));
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
int main(){
	printf("%d", to_char("01100001"));
}