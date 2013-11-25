#include "huffmantree.h"
#define NOCHAR -2;
void error(){
	perror("Error");
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
huffmantree* huffmantree_init_from_list_iter(iterator* iter){
	huffmantree* tree=malloc(sizeof(huffmantree));
	if(!tree) error();
	char buffer[CHAR_BIT+1];
	char* bufferptr=buffer;
	if(linkedlist_iteratorhasnext(iter))
		buffer[0]=*(char*)linkedlist_iteratornext(iter);
	else perror("something's wrong");
	char firstbit=bufferptr[0]-'0';
	if(!firstbit){
		tree->left=huffmantree_init_from_list_iter(iter);
		tree->right=huffmantree_init_from_list_iter(iter);
		tree->size=tree->left->size+tree->right->size+1;
	}
	else{
		for(int i=0;i<CHAR_BIT;++i){
			if(linkedlist_iteratorhasnext(iter))
				bufferptr[i]=*(char*)linkedlist_iteratornext(iter);
			else perror("something's wrong");
		}
		bufferptr[CHAR_BIT]=0;
		tree->c=to_char(bufferptr);
		//printf("%s\n",bufferptr);
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
	//printf("%s %d\n",val,n);
	return val;
}
huffmantree* huffmantree_init_from_stream(FILE* stream){
	linkedlist* list=linkedlist_init(sizeof(char));
	int num0s=0;
	int num1s=0;
	char firstbit;
	char b;
	int c;
	int mask=1<<(CHAR_BIT-1);
	while(1>(num1s-num0s)){
		c=fgetc(stream);
		for(int i=0;i<CHAR_BIT;++i){
			if(c&mask){
				b='1';
				num1s++;
				linkedlist_addend(list,&b);
				for(int j=0;j<CHAR_BIT;++j){
					c<<=1;
					i++;
					if(i>=CHAR_BIT){
						c=fgetc(stream);
						i=0;
					}
					if(c&mask) b='1';
					else b='0';
					linkedlist_addend(list,&b);
				}
			}
			else{
				b='0';
				num0s++;
				linkedlist_addend(list,&b);
			}
			c<<=1;
		}
		if(num1s>(mask<<1)){
			error();
		}
	}
	//printf("%d\n",linkedlist_size(list));
	iterator* iter=linkedlist_iterator(list);
	huffmantree* tree=huffmantree_init_from_list_iter(iter);
	linkedlist_freeiter(iter);
	linkedlist_free(list);
	return tree;
}
void huffmantree_tostringhelp(huffmantree* tree,char* array){
	if (huffmantree_isleaf(tree)){
		char* str=tobinary(tree->c);
		strcat(array,"1");
		strcat(array,str);
		free(str);
	}
	//printf("%c%c\n",tree->left->c,tree->right->c);
	else{
		strcat(array,"0");
		huffmantree_tostringhelp(tree->left,array);
		huffmantree_tostringhelp(tree->right,array);
	}
}
char* huffmantree_tostring(huffmantree* tree){
	char* array=malloc(sizeof(char)*CHAR_BIT*(tree->size+1));
	array[0]=0;
	huffmantree_tostringhelp(tree,array);
	return array;
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