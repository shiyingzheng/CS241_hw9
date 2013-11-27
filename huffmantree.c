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
	for(i=CHAR_BIT-1;i>=0;i--){
		sum=sum+(string[i]-'0')*(int)pow(2,k);
		k++;
	}
	return sum;
}
/*
 * creates a huffman tree from an iterator from a list of '1' chars and '0' chars
 */
huffmantree* huffmantree_init_from_list_iter(iterator* iter){
	huffmantree* tree=malloc(sizeof(huffmantree));//creates a tree
	if(!tree) error();
	char buffer[CHAR_BIT+1];//makes a character array of size CHAR_BIT+1 to allow CHAR_BIT bits and a null character
	char* bufferptr=buffer;
	if(linkedlist_iteratorhasnext(iter))//it should always have next but you still should check
		buffer[0]=*(char*)linkedlist_iteratornext(iter);
	else perror("something's wrong");
	char firstbit=bufferptr[0]-'0';//first bit tells you whether the current node is a leaf.
	if(!firstbit){//then this node is an internal node and you have to make its children now
		tree->left=huffmantree_init_from_list_iter(iter);//the point of having an iterator is so that all the functions
		tree->right=huffmantree_init_from_list_iter(iter);//can use it and move its position
		tree->size=tree->left->size+tree->right->size+1;//the size of a node is 1+left's size+right's size
	}
	else{//this node must be a leaf.
		for(int i=0;i<CHAR_BIT;++i){//get the next char_bit bits
			if(linkedlist_iteratorhasnext(iter))
				bufferptr[i]=*(char*)linkedlist_iteratornext(iter);
			else perror("something's wrong");
		}
		bufferptr[CHAR_BIT]=0;//a null character at the end of a string
		tree->c=to_char(bufferptr);//get the character for the binary digits
		//printf("%s\n",bufferptr);
		tree->left=NULL;//if this node is a leaf its children should be null
		tree->right=NULL;
		tree->size=1;//since this node is a leaf its size should be one
//		printf("tree->c %c \n", tree->c);
	}
	tree->count=0;//the frequencies are irrelevant once we already know the tree
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
	linkedlist* list=linkedlist_init(sizeof(char));//this method puts all the bits into a list
	int num0s=0;//count the number of 0s; or internal nodes
	int num1s=0;//count the number of 1s; or external(leaf) nodes
	char firstbit;
	char b;//a "boolean"
	int c;//the character to get from fgetc
	int mask=1<<(CHAR_BIT-1);//the most significant bit in a character
	while(1>(num1s-num0s)){//when the number of 1's equals the number of 0s+1, the tree is constructed
		c=fgetc(stream);//get the first character
		for(int i=0;i<CHAR_BIT;++i){//iterate through the bits of the character
			if(c&mask){//gets the most significant bit
				b='1';
				num1s++;//the most significant bit is 1 so we've seen a one. now we get the next char_bit bits
				linkedlist_addend(list,&b);//put this one at the end first
				for(int j=0;j<CHAR_BIT;++j){//read char_bit bits.
					c<<=1;//shift c over
					i++;//we're shifting our position in c so we do this to keep track of it
					if(i>=CHAR_BIT){//then we've finished c so we need a new char
						c=fgetc(stream);//get the next character
						i=0;//our position in c is 0 so i should also be 0
					}
					if(c&mask) b='1';
					else b='0';
					linkedlist_addend(list,&b);//add the character into the lsit
				}
			}
			else{//we have gotten a 0
				b='0';
				num0s++;
				linkedlist_addend(list,&b);
			}
			c<<=1;
		}
		if(num1s>(mask<<1)){//mask<<1 is actually just 2^char_bit. if there are more than 2^char(bit) nodes then
							//we're representing more characters than exist in the character range
			error();
		}
	}
	//printf("%d\n",linkedlist_size(list));
	iterator* iter=linkedlist_iterator(list);//get the iterator
	huffmantree* tree=huffmantree_init_from_list_iter(iter);//construct the tree with the iterator
	linkedlist_freeiter(iter);//free the iterator
	linkedlist_free(list);//free the list
	return tree;//return the tree
}
void huffmantree_tostringhelp(huffmantree* tree,char* array){
	if (huffmantree_isleaf(tree)){
		char* str=tobinary(tree->c);
		strcat(array,"1");
		strcat(array,str);
		free(str);
		printf("what");
	}
	//printf("%c%c\n",tree->left->c,tree->right->c);
	else{
		printf("mhm");
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
char* huffmantree_tobits(huffmantree* tree){
	char* array=malloc(sizeof(char)*(tree->size+1));
	char* string=huffmantree_tostring(tree);
	int i;
	for(i=0;i<strlen(string)/CHAR_BIT+1;++i){
		array[i]=to_char(string+CHAR_BIT*i);
	}
	array[i]=0;
	free(string);
	return array;
}
int huffmantree_isleaf(huffmantree* tree){
	return((!tree->left)&&(!tree->right));
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