#include "huffmantree.h"

//Have a global variable to tell us where we are in the bitarray, a fake stream of bits. 
char* eof;

linkedlist* char_to_bits(FILE* stream){
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
}

huffmantree* read_tree(linkedlist* list){
	//TODO 
	iterator* iter=linkedlist_iterator(list);
	huffmantree_init_from_list_iter(iterator* iter)
	return NULL;
}

void find_eof(linkedlist* list){
	//TODO
	//Call this method immediately after read_tree().
	//While the current tree is not a leaf, 
	//	If the current character in list is a '0', go to the left child of the current tree; otherwise go right.
	//	Accordingly, append a '0' or a '1' to eof, the global constant.
}

void print_file(linkedlist* list, huffmantree* tree){
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
}

int main(){
	return 0;
	//Take care of input and output here. Call all the methods in the order from top to bottom, so that we are reading
	//in bits in the right order.
}