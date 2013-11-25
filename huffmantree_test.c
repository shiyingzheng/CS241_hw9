#include "huffmantree.h"
int main(){
	FILE* f = fopen("test","r");
	huffmantree* tree=huffmantree_init_from_stream(f);
	char * treestr=huffmantree_tostring(tree);
	char* treebits=huffmantree_tobits(tree);
	printf("%s\n%s\n",treestr,treebits);
	free(treestr);
	free(treebits);
	huffmantree_free(tree);
	fclose(f);
}