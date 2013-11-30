#include "huffmantree.h"
#define LINE_MAX 1024
int cmp(void* a, void* b){
	huffmantree* aa=*(huffmantree**)a;
	huffmantree* bb=*(huffmantree**)b;
	return aa->count-bb->count;
}

huffmantree* frequency(FILE * stream){
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
			//printf("%d\n",i);
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
char* mystrdup(char* string,int size){
	char* newstr=malloc(sizeof(char)*size);
	if(!newstr) error();
	int i=0;
	while(string[i]){
		newstr[i]=string[i];
		i++;
	}
	newstr[i]=0;
	//printf("%s %s\n",string,newstr);
	return newstr;
}
void filltable(huffmantree* tree, char** array, char* path){
	if (huffmantree_isleaf(tree)){
		int c=tree->c;
		//printf("This is the number in the tree %d\n",c);
		array[c]=path;
	}
	else{
		char* left_path=mystrdup(path,strlen(path)+2);
		char* right_path=mystrdup(path, strlen(path)+2);
		//printf("%d %d\n",strlen(path),strlen(left_path));
		strcat(left_path,"0");
		strcat(right_path, "1");
		filltable(tree->left, array, left_path);
		filltable(tree->right, array, right_path);
		free(left_path);
		free(right_path);
	}
}

void fill(huffmantree* tree, char** array){
	int slot_size=((tree->size-1)/2+1)*sizeof(char); 
	char* path=malloc(slot_size);
	path[0]=0;
	filltable(tree, array, path);
}
void free_table(char** t){
	for(int i=0;i<=CHAR_BIT;++i){
		free(t[i]);
	}
	free(t);
}
char** table(huffmantree* tree){
	int size=(int)pow(2,CHAR_BIT)+1;
	int slot_size=((tree->size-1)/2+1)*sizeof(char); 
	char** array=malloc(sizeof(char*)*size);
	for(int i=0;i<size;++i){
		array[i]=malloc(slot_size);
		array[i][0]=0;
	}
	fill(tree, array);
	return array;
}
linkedlist* bits(huffmantree* tree, FILE* f){
	linkedlist* list=linkedlist_init(sizeof(char));
	char* treestring=huffmantree_tostring(tree);
	char b;
	int i=0;
	while(treestring[i]){
		b=treestring[i];
		linkedlist_addend(list,&b);
		//printf("TREE %c\n",b);
		i++;
	}
	char** t=table(tree);
	int c;
	char* charstr;
	charstr=t[1<<CHAR_BIT];
	//printf("%d\n",1<<CHAR_BIT);
	i=0;
	while(charstr[i]){
		b=charstr[i];
		linkedlist_addend(list,&b);
		//printf("EOF %c\n",b);
		i++;
	}
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
	return(list);
}
int main(int argc, char *argv[]){
	FILE * f=fopen("meow","r");
	huffmantree* tree=frequency(f);
	rewind(f);
	linkedlist* list=bits(tree,f);
	iterator* iter=linkedlist_iterator(list);
	while(linkedlist_iteratorhasnext(iter)){
		printf("%c",*(char*)linkedlist_iteratornext(iter));
	}
	printf("\n");
	huffmantree_free(tree);
	linkedlist_free(list);
	linkedlist_freeiter(iter);
	fclose(f);
	/*char* line=malloc(sizeof(char)*LINE_MAX);
	getline(&line,LINE_MAX,f);
	huffmantree* tree = frequency(f);
	char** t=table(tree);
	int numextraptr[1];
	char* bits=huffmantree_tobits(tree,numextraptr);
	int numextra=*numextraptr;
	int mask=1<<(numextra-1)
	char lastchar=bits[strlen(bits)-2];
	char* eofstring=table[1<<CHAR_BIT];
	int i=0;
	char* stringtoprint=eofstring;
	while(numextra){
		if(!stringtoprint[i]){
			if(eofstring==stringtoprint){
				stringtoprint=line;
				i=0;
			}
			else(eofstring!=stringtoprint){
				getline(&line,LINE_MAX,f);
				i=0;
			}
		}
		lastchar=lastchar|(mask&&eofstring[i]);
		mask>>=1;
		numextra--;
		i++;
	}
	printf("%s",bits);
	while()
	huffmantree_free(tree);
	
	fclose(f);*/
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