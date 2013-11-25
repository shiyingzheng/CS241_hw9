#include "huffmantree.h"



int cmp(void* a, void* b){
	huffmantree a=(*huffmantree)a;
	huffmantree b=(*huffmantree)b;
	return a->count-b->count;
}

sortedlist* frequency(FILE * stream){
	int array[pow(2,CHAR_BIT)+1];
	int i;
	int c;
	sortedlist* list=sortedlist_init(sizeof(huffmantree), cmp);
	huffmantree* min1;
	huffmantree* min2;
	huffmantree* temp;
	huffmantree* max;
	for (i=0;i<=CHAR_BIT;i++){
		array[i]=0;
	}
	while((c=fgetc(stream))!=EOF){
		array[c]++;
	}
	array[CHAR_BIT]=1; //EOF
	for (i=0;i<=CHAR_BIT;i++){
		if (array[i]){
			huffmantree* tree=huffmantree_init();
			tree->c=i;
			tree->count=array[i];
			sortedlist_add(list,tree);
		}
	}
	while (sortedlist_size(list)>1){
		min1=sortedlist_rm_min(list);
		min2=sortedlist_rm_min(list);
		temp=huffmantree_init();
		temp->left=min1;
		temp->right=min2;
		temp->count=min1->count+min2->count;
		sortedlist_add(list, temp);
	}
	return list;
}

int main(int argc, char *argv[]){
/*	char* infile;
	char* outfile;
	if (argc>1){
		infile=argv[1];
		outfile=argv[2];

	}
	else if (argc==1){
		infile=argv[1];
	}
	else {

	}*/

}