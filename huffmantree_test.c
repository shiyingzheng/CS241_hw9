#include "huffmantree.h"
int main(){
	FILE* f = fopen("test","r");
	huffmantree* tree=huffmantree_init_from_stream(f);
	printf("%s\n",huffmantree_tostring(tree));
	huffmantree_free(tree);
}