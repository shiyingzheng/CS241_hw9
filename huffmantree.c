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
	tree->size=0;
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
huffmantree* huffmantree_init_from_stream(FILE* stream){
	huffmantree* tree=malloc(sizeof(huffmantree));
	if(!tree) error();
	char buffer[CHAR_BIT+1];
	char* bufferptr=buffer;
	fgets(bufferptr,2,stream);
	char firstbit=bufferptr[0]-'0';
	if(!firstbit){
		tree->left=huffmantree_init_from_stream(stream);
		tree->right=huffmantree_init_from_stream(stream);
		tree->size=tree->left->size+tree->right->size+1;
	}
	else{
		fgets(bufferptr,CHAR_BIT+1,stream);
		tree->c=to_char(bufferptr);
		tree->left=NULL;
		tree->right=NULL;
		tree->size=1;
//		printf("tree->c %c \n", tree->c);
	}
	tree->count=0;
	return tree;
}
char* tobinary(unsigned int n){
	unsigned int c=n;
	int i;
    int mask=1;
    char* val=malloc(sizeof(char)*CHAR_BIT+1);
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
	val[CHAR_BIT]=0;
	printf("%s %d\n",val,n);
	return val;
}
char* huffmantree_tostring(huffmantree* tree){
	if (huffmantree_isleaf(tree)) {
		return strcat(strr,tobinary(tree->c));
	}
//	printf("%c%c\n",tree->left->c,tree->right->c);
	char* str = strcat(huffmantree_tostring(tree->left), huffmantree_tostring(tree->right));\
	char* strr="0";
	return strcat(strr,str);
}
int huffmantree_isleaf(huffmantree* tree){
	return(!tree->left&&!tree->right);
}
void huffmantree_free(huffmantree* tree){
	if (tree->right){
		huffmantree_free(tree->right);
	}
	if (tree->left){
		huffmantree_free(tree->left);
	}
	free(tree);
}
/*int main(){
	printf("%d", to_char("01100001"));
}*/