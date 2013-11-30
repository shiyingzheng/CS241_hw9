#include "huffmantree.h"

//Have a global variable to tell us where we are in the bitarray, a fake stream of bits. 
static int counter=0;
const char* eof;

char* char_to_bits(FILE* f){
	//TODO
	//This method reads in each character of the file and converts it to its binary representation
	//and then append to the array
	//Each element in the array is a character of either 0 or 1
	//This array is used in various methods below. It simulates a stream of bits, 
	//so that we don't need to worry about keeping track of what's left in the current character in 
	//the real stream.
}

huffmantree* read_tree(char* array){
	//TODO 
	//Call this method on the array returned from char_to_bits()
	//This is a recursive method. 
	//If the current character is a '0', make a huffmantree*, the left node is whatever returned by read_tree(array),
	//	and the right node is whatever returned by read_tree(array)
	//	Increment counter by 1.
	//	return the tree.
	//Otherwise, make a huffmantree*, and getthe next CHAR_BIT characters from the array, and convert the bits to
	//	an actual character. (See to_char() in huffmantree.c)
	//	Make tree->c the character.
	//	Increment counter by CHAR_BIT+1.
	//	return the tree.
	//According to this algorithm, the function will stop recursing when there is no more leaves left in the tree.
}

void find_eof(char* array){
	//TODO
	//Call this method immediately after read_tree().
	//While the current tree is not a leaf, 
	//	If the current character in array is a '0', go to the left child of the current tree; otherwise go right.
	//	Accordingly, append a '0' or a '1' to eof, the global constant.
	//	Increment counter by 1.
}

void print_file(char* array, huffmantree* tree){
	//TODO
	//Before using this method, please set the global constant root to be the root of the whole tree.
	//This method is used after find_eof().
	//The algorithm is:
	//While we haven't reached eof,
	//	while the current tree is not a leaf,
	//		If the current character in array is a '0', go to the left child of the current tree; otherwise go right.
	//		Increment counter by 1.
	//	(Now we have found a leaf.) 
	//	if its character and its path are not indicating that it is eof, 
	//		print tree->c.
	//		Increment counter by CHAR_BIT+1.
	//	otherwise, we have reached eof and we need to break from the loop.
}

int main(){
	//Take care of input and output here. Call all the methods in the order from top to bottom, so that we are reading
	//in bits in the right order.
}