#include "huffmantree.h"

//Have a global variable to tell us where we are in the bitarray, a fake stream of bits. 
static int counter=0;
const char* eof;

linkedlist* char_to_bits(FILE* f){
	//TODO
	//This method reads in each character of the file and converts it to its binary representation
	//and then append to the list
	//Each element in the list is a character of either 0 or 1
	//This list is used in various methods below. It simulates a stream of bits, 
	//so that we don't need to worry about keeping track of what's left in the current character in 
	//the real stream.
}

huffmantree* read_tree(linkedlist* list){
	//TODO 
	//huffmantree_init_from_list_iter(iterator* iter)
}

void find_eof(linkedlist* list){
	//TODO
	//Call this method immediately after read_tree().
	//While the current tree is not a leaf, 
	//	If the current character in list is a '0', go to the left child of the current tree; otherwise go right.
	//	Accordingly, append a '0' or a '1' to eof, the global constant.
	//	Increment counter by 1.
}

void print_file(linkedlist* list, huffmantree* tree){
	//TODO
	//This method is used after find_eof().
	//The algorithm is:
	//While we haven't reached eof,
	//	while the current tree is not a leaf,
	//		If the current character in list is a '0', go to the left child of the current tree; otherwise go right.
	//		Increment counter by 1.
	//	(Now we have found a leaf.) 
	//	if the leaf's character and its path are not indicating that it is eof, 
	//		print tree->c.
	//		Increment counter by CHAR_BIT+1.
	//	otherwise, we have reached eof and we need to break from the loop.
}

int main(){
	//Take care of input and output here. Call all the methods in the order from top to bottom, so that we are reading
	//in bits in the right order.
}