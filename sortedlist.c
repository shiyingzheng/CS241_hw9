/*
 * Shiying Zheng and Ben Stern, hw9
 * A sorted list built upon a linkedlist.
 */

#include "sortedlist.h"
/*
 * Initialize a sortedlist.
 * Takes in an integer which is the size of each element of the sortedlist,
 *	and a comparator which takes in two void pointers, compare them, and 
 * 	returns an int.
 * Returns a pointer to a sorted list.
 */
sortedlist* sortedlist_init(int datasize, int (*cmp)(void* a, void* b)){
	sortedlist* this=malloc(sizeof(sortedlist));
	if(!this){
		exit(EXIT_FAILURE);
	}
	this->list=linkedlist_init(datasize);
	this->cmp=cmp;
	this->iter=linkedlist_iterator(this->list);
	return this;
}
/*
 * Takes in a pointer to the sortedlist.
 * Returns the size of the sortedlist.
 */
int sortedlist_size(sortedlist* this){
	return linkedlist_size(this->list);
}
/*
 * Adds data to the sortedlist. The list will remain sorted.
 * Takes in a pointer to the sortedlist and a void pointer to the data to add.
 */
void sortedlist_add(sortedlist* this,void* data){
	//Using the iterator of the linkedlist inside of the sortedlist.
	int comparison;
	if(linkedlist_iteratorhasnext(this->iter)){
		comparison=this->cmp(data,linkedlist_iteratornext(this->iter));
		if(comparison>0){
			while(comparison>0&&linkedlist_iteratorhasnext(this->iter)){
				comparison=this->cmp(data,linkedlist_iteratornext(this->iter));
			}
			if(comparison<0) linkedlist_iteratorprev(this->iter);
		}
		else{
			while(comparison<0&&linkedlist_iteratorhasprev(this->iter)){
				comparison=this->cmp(data,linkedlist_iteratorprev(this->iter));
			}
			if(comparison>0) linkedlist_iteratornext(this->iter);
		}
	}
	else if(linkedlist_iteratorhasprev(this->iter)){
		comparison=this->cmp(data,linkedlist_iteratorprev(this->iter));
		while(comparison<0&&linkedlist_iteratorhasprev(this->iter)){
			comparison=this->cmp(data,linkedlist_iteratorprev(this->iter));
		}
		if(comparison>0) linkedlist_iteratornext(this->iter);
	}
	linkedlist_iteratoradd(this->iter,data);
}
/*
 * Removes data from the sortedlist. 
 * Takes in a pointer to the sortedlist,
 * and a void pointer to the data to remove.
 */
void* sortedlist_rm(sortedlist* this, void* data){
	//Using the linkedlist iterator.
	int comparison;
	void * rmdata;
	if (linkedlist_iteratorhasnext(this->iter)){
		comparison=this->cmp(data, linkedlist_iteratornext(this->iter));
		if (comparison>0){
			while(comparison>0&&linkedlist_iteratorhasnext(this->iter)){
				comparison=this->cmp(data, linkedlist_iteratornext(this->iter));
			}
			if (comparison<0) return NULL;
		}
		else {
			while(comparison<0&&linkedlist_iteratorhasprev(this->iter)){
				comparison=this->cmp(data, linkedlist_iteratorprev(this->iter));
			}
			if (comparison>0) return NULL;
		}
	}
	else if (linkedlist_iteratorhasprev(this->iter)){
		comparison=this->cmp(data,linkedlist_iteratorprev(this->iter));
		while(comparison<0&&linkedlist_iteratorhasprev(this->iter)){
			comparison=this->cmp(data,linkedlist_iteratorprev(this->iter));
		}
		if(comparison>0) return NULL;
	}
	if(linkedlist_iteratorhasnext(this->iter)){
		linkedlist_iteratornext(this->iter);
	}
	rmdata=linkedlist_iteratorrm(this->iter);
	return rmdata;
}
/*
 * Takes in a pointer to the sortedlist.
 * Returns the min data in the list.
 */
void* sortedlist_get_min(sortedlist* this){
	return linkedlist_getfront(this->list);
}
/*
 * Takes in a pointer to the sortedlist.
 * Returns the max data in the list.
 */
void* sortedlist_get_max(sortedlist* this){
	return linkedlist_getend(this->list);
}
/*
 * Removes the min data from the sortedlist.
 * Takes in a pointer to the sortedlist.
 * Returns the removed data in the list.
 */
void* sortedlist_rm_min(sortedlist* this){
	this->iter->position=0;
	void* returnthis=linkedlist_rmfront(this->list);
	this->iter->current=this->iter->list->head;
	return returnthis;
}
/*
 * Removes the max data from the sortedlist.
 * Takes in a pointer to the sortedlist.
 * Returns the removed data in the list.
 */
void* sortedlist_rm_max(sortedlist* this){
	this->iter->position=this->list->size;
	void* returnthis=linkedlist_rmend(this->list);
	this->iter->current=this->iter->list->tail;
	return returnthis;
}

/* Iterator Methods */


/*
 * Returns an iterator over the sortedlist.
 */
iterator* sortedlist_iterator(sortedlist* this){
	return linkedlist_iterator(this->list);
}
/*
 * Takes in a pointer to an iterator.
 * Returns 0 if there is not a next element,
 * otherwise a non-zero value.
 */
int sortedlist_iteratorhasnext(iterator* iter){
	return linkedlist_iteratorhasnext(iter);
}
/*
 * Takes in a pointer to an iterator.
 * Returns 0 if there is not a previous element,
 * otherwise a non-zero value.
 */
int sortedlist_iteratorhasprev(iterator* iter){
	return linkedlist_iteratorhasprev(iter);
}
/*
 * Takes in a pointer to an iterator.
 * Returns the next element.
 */
void* sortedlist_iteratornext(iterator* iter){
	return linkedlist_iteratornext(iter);
}
/*
 * Takes in a pointer to an iterator.
 * Returns the previous element.
 */
void* sortedlist_iteratorprev(iterator* iter){
	return linkedlist_iteratorprev(iter);
}
/*
 * Frees the memory allocated for the iterator.
 * Takes in a pointer to the iterator.
 */
void sortedlist_freeiter(iterator* iter){
	linkedlist_freeiter(iter);
}
/*
 * Frees the memory allocated for a sortedlist.
 * Takes in a pointer to the sortedlist.
 */
void sortedlist_free(sortedlist* list){
	linkedlist_freeiter(list->iter);
	linkedlist_free(list->list);
	free(list);
}