#include "huffmantree.h"

int cmp(void* a, void* b){
	huffmantree* aa=*(huffmantree**)a;
	huffmantree* bb=*(huffmantree**)b;
	return aa->count-bb->count;
}

huffmantree* frequency(FILE * stream){
	int size=(int)pow(2,CHAR_BIT)+1;
	int array[size];
	int i;
	int c;
	sortedlist* list=sortedlist_init(sizeof(huffmantree*), cmp);
	huffmantree* min1;
	huffmantree* min2;
	huffmantree* temp;
	huffmantree* max;
	for (i=0;i<size;i++){
		array[i]=0;
	}
	while(!feof(stream)){
		c=fgetc(stream);
		array[c]=array[c]+1;
	}
	array[size-1]=1; //EOF
	for (i=0;i<size;i++){
		if (array[i]){
			huffmantree* tree=huffmantree_init();
			tree->c=i;
			tree->count=array[i];
			sortedlist_add(list,&tree);
			//huffmantree_free(tree);
		}
	}
	/*iterator* iter=sortedlist_iterator(list);
	while(sortedlist_iteratorhasnext(iter)){
		temp=*(huffmantree**)sortedlist_iteratornext(iter);
		//printf("%c %d\n",temp->c,temp->count);
	}
	sortedlist_freeiter(iter);*/
	while (sortedlist_size(list)>1){
		huffmantree** m1=(huffmantree**)sortedlist_rm_min(list);
		huffmantree** m2=(huffmantree**)sortedlist_rm_min(list);
		//("list size %d\n",sortedlist_size(list));
		min1=*m1;
		//printf("iterposition %d\n",list->iter->position);
		min2=*m2;
		//printf("iterposition %d\n",list->iter->position);
		//printf("list size %d\n",sortedlist_size(list));
		//iterator* iter=sortedlist_iterator(list);
		//.while(sortedlist_iteratorhasnext(iter)){
			//temp=*(huffmantree**)sortedlist_iteratornext(iter);
			//printf("%p %c %d\n\n",iter->current,temp->c,temp->count);
		//}
		//sortedlist_freeiter(iter);
		temp=huffmantree_init();
		temp->left=min1;
		temp->right=min2;
		temp->count=min1->count+min2->count;
		temp->size=min1->size+min2->size+1;
		//printf("%d+%d=%d\n",min1->count,min2->count,temp->count);
		sortedlist_add(list, &temp);
		free(m1);
		free(m2);
	}
	huffmantree** m=(huffmantree**)sortedlist_rm_max(list);
	max=*m;
	sortedlist_free(list);
	free(m);
	return max;
}

int main(int argc, char *argv[]){
	FILE * f=fopen("meow","r");

	huffmantree* tree = frequency(f);
	char* string=huffmantree_tostring(tree);
	printf("%s\n",string);
	free(string);
	unsigned char* bitstring=huffmantree_tobits(tree);
	for(int i=0;bitstring[i]!=0;++i){
		printf("%d ",bitstring[i]);
	}
	printf("\n");
	free(bitstring);
	huffmantree_free(tree);
	//free(string);
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