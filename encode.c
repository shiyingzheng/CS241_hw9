/*
 * Shiying Zheng and Ben Stern hw9
 * The encode program
 */
#include "huffmantree.h"
 /*
  * A struct for a string. We need this because there could be a null character
  * in the encoded file.
  */
typedef struct string{
	char* chars;
	int size;
} String;
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
 * Initializes a string.
 * Returns a pointer to a string.
 */
String* string_init(){
	String* s=malloc(sizeof(String));
	if(!s) error();
	s->size=0;
	return s;
}
/*
 * Prints the string to file.
 * Takes in a FILE pointer to print to and a String pointer to print.
 */
void fprintstring(FILE* out,String* string){
	for(int i=0;i<string->size;++i){
		fprintf(out,"%c",string->chars[i]);
	}
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
			//printf("%d %d\n",i,array[i]);
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
	//printf("%s %s %d\n",string,newstr,size);
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
		//printf("This is the number in the tree %d\n",c);
		//array[c]=mystrdup(path,strlen(path)+1);
		memcpy(array[c],path,strlen(path)+1);
	}
	else{
		char* left_path=mystrdup(path,strlen(path)+2);
		char* right_path=mystrdup(path, strlen(path)+2);
		strcat(left_path,"0");
		strcat(right_path, "1");
		//printf("%lu %lu\n",strlen(path),strlen(left_path));
		//printf("%s %s\n", left_path, right_path);
		//printf("%p %p, %p\n",path, left_path, right_path);
		filltable(tree->left, array, left_path);
		filltable(tree->right, array, right_path);
		free(left_path);
		free(right_path);
		//printf("freed %p, %p\n",left_path, right_path);
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
	//printf("Slot size is %d\n",slot_size);
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
linkedlist* bits(huffmantree* tree, FILE* f){
	linkedlist* list=linkedlist_init(sizeof(char));

	//Add the tree representation to the list.
	char* treestring=huffmantree_tostring(tree);
	char b;
	int i=0;
	while(treestring[i]){
		b=treestring[i];
		linkedlist_addend(list,&b);
		//printf("TREE %c\n",b);
		i++;
	}
	//printf("%d\n",linkedlist_size(list));

	//Gets the table to look up when encoding the original file.
	char** t=table(tree);
	int c;
	char* charstr;
	charstr=t[1<<CHAR_BIT]; //This is the last entry of the table. Follow to get to the EOF node.
	//printf("%d\n",1<<CHAR_BIT);

	//Adds the path to EOF to the list.
	i=0;
	while(charstr[i]){
		b=charstr[i];
		linkedlist_addend(list,&b);
		//printf("EOF %c\n",b);
		i++;
	}

	//Encoding the original file.
	while(EOF!=(c=fgetc(f))){
		i=0;
		charstr=t[c];
		//printf("%c %s\n",c,t[c]);
		while(charstr[i]){
			b=charstr[i];
			linkedlist_addend(list,&b);
			//printf("FILE %c\n",b);
			i++;
		}
	}
	charstr=t[1<<CHAR_BIT];
	i=0;
	while(charstr[i]){
		b=charstr[i];
		linkedlist_addend(list,&b);
		//printf("EOF %c\n",b);
		i++;
	}
	free_table(t);
	free(treestring);
	//printf("%d\n",linkedlist_size(list));
	return(list);
}
/*
 * Converts a linkedlist of 0's and 1's, which are the bit representation of the encoded file,
 * to an String.
 * Takes in a pointer to a linkedlist.
 * Returns a pointer to a String.
 */
String* list_to_string(linkedlist* list){
	int remain=CHAR_BIT-(linkedlist_size(list)%CHAR_BIT);
	int binary_size=linkedlist_size(list)+remain;
	//printf("%d\n",binary_size);
	char* string=malloc(sizeof(char)*(binary_size/CHAR_BIT+1));
	int counter=0;
	int sum=0;
	int i=0;
	iterator* iter=linkedlist_iterator(list);
	string[0]=0;
	while (counter<binary_size){
		if (linkedlist_iteratorhasnext(iter)){
			sum=sum*2+(*(char*)linkedlist_iteratornext(iter)-'0');
		}
		else{
			sum=sum*2;
		}
		counter++;
		if (counter%CHAR_BIT==0){
			string[i]=sum;
			//printf("%d %c\n",sum, sum);
			i++;
			sum=0;
		}
	}
	//printf("%d\n%s\n",counter,string);
	string[i]=0;
	linkedlist_freeiter(iter);
	String* str=string_init();
	str->size=binary_size/CHAR_BIT;
	str->chars=string;
	return str;
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
	/*char* s=huffmantree_tostring(tree);
	printf("%s\n",s);
	free(s);*/
	rewind(f);
	linkedlist* list=bits(tree,f);
	String* string=list_to_string(list);
	if (argc>2){
		FILE* out=fopen(argv[2], "w");
		fprintstring(out, string);
		fclose(out);
	}
	else fprintstring(stdout, string);

	huffmantree_free(tree);
	linkedlist_free(list);
	free(string->chars);
	free(string);
	fclose(f);
	exit(0);
}