/*
 * Shiying Zheng and Ben Stern hw9
 * The encode program
 */
#include "huffmantree.h"
 /*
  * The comparator for huffmantree.
  * Takes in two void pointers, returns an integer which is positive if 
  *  a's count is bigger than b's count, negative if smaller, and zero if equal.
  */
int cmp(void* a, void* b){
	huffmantree* aa=*(huffmantree**)a;
	huffmantree* bb=*(huffmantree**)b;
	return aa->count-bb->count;
}
/*
 * Constructs the huffmantree from the file. Rewind after using the function.
 * Takes in a pointer to a FILE.
 * Returns a pointer to a huffmantree.
 */
huffmantree* frequency_tree(FILE * stream){
	int size=(int)pow(2,CHAR_BIT)+1;
	int array[size];
	int i;
	unsigned int c;
	sortedlist* list=sortedlist_init(sizeof(huffmantree*), cmp);
	huffmantree* min1;
	huffmantree* min2;
	huffmantree* temp;
	huffmantree* max;
	for (i=0;i<size;++i){
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
			sortedlist_add(list,&tree);
		}
	}
	while (sortedlist_size(list)>1){
		huffmantree** m1=(huffmantree**)sortedlist_rm_min(list);
		huffmantree** m2=(huffmantree**)sortedlist_rm_min(list);
		min1=*m1;
		min2=*m2;
		temp=huffmantree_init();
		temp->left=min1;
		temp->right=min2;
		temp->count=min1->count+min2->count;
		temp->size=min1->size+min2->size+1;
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
/*
 * Makes a duplicate of a string. We need this because when we call strcat, it will
 * need a larger size than just the original string.
 * Takes in a character pointer and an integer which is the size of the character
 * array.
 * Returns a pointer to a character array.
 */
char* mystrdup(char* string,int size){
	char* newstr=malloc(sizeof(char)*size);
	if(!newstr) error();
	int i=0;
	while(string[i]){
		newstr[i]=string[i];
		i++;
	}
	newstr[i]=0;
	return newstr;
}
/*
 * Helper method of fill. 
 * It assigns a string that represents the path to the character in the huffmantree
 *	to the corresponding slot in the table, an array of strings.
 * Takes in a pointer to a huffmantree, a pointer to character pointers, which is the
 * table, and a character array, which is the collector in the tail recursion.
 */
void filltable(huffmantree* tree, char** array, char* path){
	if (huffmantree_isleaf(tree)){
		int c=tree->c;
		memcpy(array[c],path,strlen(path)+1);
	}
	else{
		char* left_path=mystrdup(path,strlen(path)+2);
		char* right_path=mystrdup(path, strlen(path)+2);
		strcat(left_path,"0");
		strcat(right_path, "1");
		filltable(tree->left, array, left_path);
		filltable(tree->right, array, right_path);
		free(left_path);
		free(right_path);
	}
}
/*
 * Fills a table of paths to characters in the tree.
 * Takes in a pointer to a huffmantree and a pointer to character pointers.
 */
void fill(huffmantree* tree, char** array){
	int slot_size=((tree->size-1)/2+1)*sizeof(char); //maximum length of a path from root to leaf
	char* path=malloc(slot_size);
	path[0]=0;
	filltable(tree, array, path);
	free(path);
}
/*
 * Frees the memory allocated for the table.
 * Takes in a pointer to character pointers, which is the table.
 */
void free_table(char** t){
	for(int i=0;i<pow(2,CHAR_BIT)+1;++i){
		free(t[i]);
	}
	free(t);
}
/*
 * Constructs the table of paths to characters in the tree.
 * Takes in a pointer to a huffmantree.
 * Returns a pointer to character pointers, which is the table.
 */
char** table(huffmantree* tree){
	int size=(int)pow(2,CHAR_BIT)+1;
	int slot_size=((tree->size-1)/2+1)*sizeof(char); //maximum length of a path from root to leaf
	char** array=malloc(sizeof(char*)*size);
	for(int i=0;i<size;++i){
		array[i]=malloc(slot_size);
		array[i][0]=0;
	}
	fill(tree, array);
	return array;
}
/*
 * Constructs a linkedlist of bits of encoded file.
 * Takes in a pointer to a huffmantree and a FILE pointer.
 * Returns a pointer to the linkedlist.
 */
linkedlist* treebits(huffmantree* tree,char** t){
	linkedlist* list=linkedlist_init(sizeof(char));

	//Add the tree representation to the list.
	char* treestring=huffmantree_tostring(tree);
	char b;
	int i=0;
	while(treestring[i]){
		b=treestring[i];
		linkedlist_addend(list,&b);
		i++;
	}
	int c;
	char* charstr;
	charstr=t[1<<CHAR_BIT]; //This is the last entry of the table. Follow to get to the EOF node.

	//Adds the path to EOF to the list.
	i=0;
	while(charstr[i]){
		b=charstr[i];
		linkedlist_addend(list,&b);
		i++;
	}
	free(treestring);
	return list;
}
/*
 * extends the list by numchars*CHAR_BIT nodes, adding them from f bitwise into the end of the list
 * does not add any more if it reaches EOF
 */
void extendlist(linkedlist* list, char** t, int numchars, FILE* f){
	//Encoding the original file.
	int i;
	char b;
	char* charstr;
	int j=0;
	int c;
	while(j<numchars&&EOF!=(c=fgetc(f))){
		i=0;
		charstr=t[c];
		while(charstr[i]){
			b=charstr[i];
			linkedlist_addend(list,&b);
			i++;
		}
		j++;
	}
}
/*
 * puts the bits for the new EOF marker at the end of the list
 */
void extendlistwitheof(linkedlist* list,char** t){
	int c;
	char* charstr;
	int i;
	char b;
	charstr=t[1<<CHAR_BIT];
	i=0;
	while(charstr[i]){
		b=charstr[i];
		linkedlist_addend(list,&b);
		i++;
	}
}
/*
 * prints the bits from the linked list as chars
 * takes chars from stream as needed with extend list
 * until the file reaches EOF.
 * prints the new eof mapping as well
 * fills out the rest of the bits with 0's
 */
void printbits(huffmantree* tree,FILE* stream,FILE* out){
	char** t=table(tree);
	linkedlist* list=treebits(tree,t);
	int counter=0;
	int maxcharlength=(tree->size+1)/2;
	int sum=0;
	int i=0;
	int eof=0;
	while(linkedlist_size(list)){
		//printf("%d\n",linkedlist_size(list));
		if(linkedlist_size(list)<maxcharlength){
			if(maxcharlength>CHAR_BIT) extendlist(list,t,maxcharlength/CHAR_BIT,stream);
			else extendlist(list,t,maxcharlength,stream);
		}
		if(feof(stream)&&!eof){
			extendlistwitheof(list,t);
			eof=1;
		}
		char* frontbit=(char*)linkedlist_rmfront(list);
		sum=sum*2+(*frontbit-'0');
		free(frontbit);
		counter++;
		if (counter%CHAR_BIT==0){
			fprintf(out,"%c",sum);
			sum=0;
		}
	}
	i=0;
	while(counter%CHAR_BIT!=0){
		i++;
		sum=sum*2;
		counter++;
	}
	if(i)fprintf(out,"%c",sum);
	linkedlist_free(list);
	free_table(t);
}
/*
 * The main method.
 */
int main(int argc, char *argv[]){
	if (argc==1){
		printf("Error: No input file specified.\n");
		printf("Usage: argv[0] input_file [output_file]\n");
		printf("If no output file is specified, the program will print to standard output.\n");
		exit(3);
	}
	FILE* f = fopen(argv[1], "r");
	if (f == NULL) {
    	perror("In encode, error");
    	exit(3);
    }
	huffmantree* tree=frequency_tree(f);
	char* tree_string=huffmantree_tostring(tree);
	printf("treestring %s\n", tree_string);
	rewind(f);
	FILE* out=stdout;
	if (argc>2){
		out=fopen(argv[2], "w");
	}
	printbits(tree,f,out);
	huffmantree_free(tree);
	fclose(f);
	if(out!=stdout)fclose(out);
	exit(0);
}