#include "huffmantree.h"

int cmp(void* a, void* b){
	huffmantree* aa=(huffmantree*)a;
	huffmantree* bb=(huffmantree*)b;
	return aa->count-bb->count;
}

huffmantree* frequency(FILE * stream){
	int size=(int)pow(2,CHAR_BIT)+1;
	int array[size];
	int i;
	int c;
	sortedlist* list=sortedlist_init(sizeof(huffmantree), cmp);
	huffmantree* min1;
	huffmantree* min2;
	huffmantree* temp;
	huffmantree* max;
	for (i=0;i<size;i++){
		array[i]=0;
	}
	while((c=fgetc(stream))!=EOF){
		array[c]=array[c]+1;
	}
	array[size-1]=1; //EOF
	for (i=0;i<size;i++){
		if (array[i]){
			huffmantree* tree=huffmantree_init();
			tree->c=i;
			tree->count=array[i];
			sortedlist_add(list,tree);
			huffmantree_free(tree);
		}
	}
	while (sortedlist_size(list)>1){
		min1=(huffmantree*)sortedlist_rm_min(list);////////////
		min2=(huffmantree*)sortedlist_rm_min(list);////////////
		temp=huffmantree_init();
		temp->left=min1;
		temp->right=min2;
		temp->count=min1->count+min2->count;
		sortedlist_add(list, temp);////////
	}
	max=(huffmantree*)sortedlist_rm_max(list);///////////////
	sortedlist_free(list);
	return max;
}

int main(int argc, char *argv[]){
	FILE * f=fopen("meow.txt","r");

	huffmantree* tree = frequency(f);
	char* string=huffmantree_tostring(tree);
	huffmantree_free(tree);
//	free(string);
	fclose(f);
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