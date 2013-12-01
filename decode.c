#include "huffmantree.h"

void printlist(linkedlist* list){
	iterator* iter=linkedlist_iterator(list);
	while(linkedlist_iteratorhasnext(iter)){
		printf("%c",*(char*)linkedlist_iteratornext(iter));
	}
	printf("\n");
	linkedlist_freeiter(iter);
}
linkedlist* chars_to_bits(FILE* stream){
	//TODO
	//This method reads in each character of the file and converts it to its binary representation
	//and then append to the list
	//Each element in the list is a character of either 0 or 1
	//This list is used in various methods below. It simulates a stream of bits, 
	//so that we don't need to worry about keeping track of what's left in the current character in 
	//the real stream.
	linkedlist* list=linkedlist_init(sizeof(char));
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
					linkedlist_addend(list,&b);//add the character into the list
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
	return list;
}
huffmantree* read_tree(iterator* iter){
	huffmantree* tree=huffmantree_init_from_list_iter(iter);
	return tree;
}
int decode_char(linkedlist* list,huffmantree* tree){
	if(huffmantree_isleaf(tree)){
		//printf("%d\n",tree->c);
		return tree->c;
	}
	char* p=linkedlist_rmfront(list);
	char c=*p;
	free(p);
	if(c=='0')return decode_char(list,tree->left);
	return decode_char(list,tree->right);
}
void set_eof(linkedlist* list, huffmantree* tree){
	//printf("Setting EOF\n");
	if(huffmantree_isleaf(tree)){
		tree->c=EOF;
	}
	else{
		char* p=linkedlist_rmfront(list);
		char c=*p;
		free(p);
		//printf("Setting EOF to %c\n",c);
		if(c=='0')set_eof(list,tree->left);
		else set_eof(list,tree->right);

	}
}
int extend_list(linkedlist* list,int min_size,FILE* stream){
	int ch;
	int retval=0;
	char b;
	int mask=1<<(CHAR_BIT-1);
	while(linkedlist_size(list)<min_size){
		if((ch=fgetc(stream))==EOF)
			retval=EOF;
		for(int i=0;i<CHAR_BIT;++i){
			if(ch&mask) b='1';
			else b='0';
			//printf("%c\n",b);
			linkedlist_addend(list,&b);
			ch<<=1;
		}
	}
	return retval;
}
void print_file(linkedlist* list, huffmantree* tree, FILE* stream){
	//TODO
	//This method is used after find_eof().
	//The algorithm is:
	//While we haven't reached eof,
	//	while the current tree is not a leaf,
	//		If the current character in list is a '0', go to the left child of the current tree; otherwise go right.
	//	(Now we have found a leaf.) 
	//	if the leaf's character and its path are not indicating that it is eof, 
	//		print tree->c.
	//	otherwise, we have reached eof and we need to break from the loop.
	int min_size=(tree->size-1)/2;
	int c;
	extend_list(list,min_size,stream);
	while(EOF!=c&&EOF!=(c=decode_char(list,tree))){
		if(extend_list(list,min_size,stream)==EOF)
			c=EOF;
		else printf("%c",c);
	}
}

void decode_file(FILE* stream){
	linkedlist* list=chars_to_bits(stream);
	//printlist(list);
	iterator* iter=linkedlist_iterator(list);
	huffmantree* tree=read_tree(iter);
	int min_size=(tree->size-1)/2;
	while(linkedlist_iteratorhasprev(iter)){
		free(linkedlist_iteratorrm(iter));
	}
	linkedlist_freeiter(iter);
	//printlist(list);
	//printf("Stuff\n");
	if(EOF==extend_list(list,min_size,stream))printf("Something's wrong here");
	//printf("MoreStuff\n");
	set_eof(list,tree);
	//printf("MoreMoreStuff\n");
	print_file(list,tree,stream);
	//printf("This is not a joke\n");
	linkedlist_free(list);
	huffmantree_free(tree);
}
int main(){
	FILE* f=stdin;//fopen("ruff","r");
	decode_file(f);
	/*rewind(f);
	int c;
	while((c=fgetc(f))!=EOF){
		printf("%x ",c);
	}*/
	fclose(f);
	//Take care of input and output here. Call all the methods in the order from top to bottom, so that we are reading
	//in bits in the right order.
}