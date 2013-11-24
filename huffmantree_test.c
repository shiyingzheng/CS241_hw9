#include "huffmantree.h"
int main(){
	FILE* f = fopen("test","r");
	huffmantree* tree=huffmantree_init_from_stream(f);
	char * treeval=huffmantree_tostring(tree);
	free(treeval);
	printf("%s\n",treeval);
	free(treeval);
	huffmantree_free(tree);
}