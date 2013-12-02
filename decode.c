/*
 * Shiying Zheng and Ben Stern hw9 
 * The decode program.
 */

#include "huffmantree.h"
/*
 * Prints a linkedlist of characters.
 * Takes in a pointer to the linkedlist.
 */
void printlist(linkedlist* list){
	iterator* iter=linkedlist_iterator(list);
	while(linkedlist_iteratorhasnext(iter)){
		printf("%c",*(char*)linkedlist_iteratornext(iter));
	}
	printf("\n");
	linkedlist_freeiter(iter);
}
/*
 * Reads in each character of the file and converts it to its binary representation
 *  and then append to a linkedlist.
 * Takes in a FILE pointer and returns a pointer to a linkedlist.
 */
linkedlist* chars_to_bits(FILE* stream){
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
	}
	return list;
}
/*
 * Constructs a tree from the starting elements in a linkedlist.
 * Takes in a pointer to an iterator over the linkedlist, returns a pointer to a huffmantree.
 */
huffmantree* read_tree(iterator* iter){
	huffmantree* tree=huffmantree_init_from_list_iter(iter);
	return tree;
}
/*
 * Decodes a character at the front of the list of binary representation of the encoded file.
 * Takes in a pointer to the linkedlist, returns an integer which represents the character.
 */
int decode_char(linkedlist* list,huffmantree* tree){
	if(huffmantree_isleaf(tree)){
		return tree->c;
	}
	char* p=linkedlist_rmfront(list);
	char c=*p;
	free(p);
	if(c=='0')return decode_char(list,tree->left);
	return decode_char(list,tree->right);
}
/*
 * Setting the character of the right node in the huffmantree to be EOF.
 * Takes in a pointer to a linkedlist and a pointer to a huffmantree.
 */
void set_eof(linkedlist* list, huffmantree* tree){
	if(huffmantree_isleaf(tree)){
		tree->c=EOF;
	}
	else{
		char* p=linkedlist_rmfront(list);
		char c=*p;
		free(p);
		if(c=='0')set_eof(list,tree->left);
		else set_eof(list,tree->right);

	}
}
/*
 * Extends the buffer linkedlist by reading from the stream.
 * Takes in a pointer to a linkedlist, an integer which is the minimum size that the buffer should be,
 *  and a FILE pointer.
 */
void extend_list(linkedlist* list,int min_size,FILE* stream){
	int ch;
	char b;
	int mask=1<<(CHAR_BIT-1);
	while(linkedlist_size(list)<min_size){
		ch=fgetc(stream);
		for(int i=0;i<CHAR_BIT;++i){
			if(ch&mask) b='1';
			else b='0';
			linkedlist_addend(list,&b);
			ch<<=1;
		}
	}
}
/*
 * Prints out the decoded file.
 * Takes in a pointer to a linkedlist, a pointer to a huffmantree, a FILE pointer which is the input, and 
 * a FILE pointer which is the output.
 */
void print_file(linkedlist* list, huffmantree* tree, FILE* stream,FILE* out){
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
	int eof=0;
	extend_list(list,min_size,stream);
	while(EOF!=(c=decode_char(list,tree))){
		extend_list(list,min_size,stream);
		fprintf(out,"%c",c);
	}
}
/*
 * Calls the appropriate functions to decode a file. 
 * Takes in a FILE pointer which is the input, and 
 * a FILE pointer which is the output.
 */
void decode_file(FILE* stream,FILE* out){
	linkedlist* list=chars_to_bits(stream);
	iterator* iter=linkedlist_iterator(list);
	huffmantree* tree=read_tree(iter);
	int min_size=(tree->size-1)/2;
	while(linkedlist_iteratorhasprev(iter)){
		free(linkedlist_iteratorrm(iter));
	}
	linkedlist_freeiter(iter);
	extend_list(list,min_size,stream);
	set_eof(list,tree);
	print_file(list,tree,stream,out);
	linkedlist_free(list);
	huffmantree_free(tree);
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
    	perror("In decode, error\n");
    	exit(3);
    }
	
	if (argc>2){
		FILE* out=fopen(argv[2], "w");
		decode_file(f, out);
		fclose(out);
	}
	else decode_file(f, stdout);
	
	fclose(f);
}