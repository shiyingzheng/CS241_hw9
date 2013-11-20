#include "sortedlist.h"
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
void sortedlist_add(sortedlist* this,void* data){
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
void* sortedlist_rm(sortedlist* list, void* data); 
iterator* sortedlist_iterator(sortedlist* this){
	return linkedlist_iterator(this->list);
}
int sortedlist_iteratorhasnext(iterator* iter){
	return linkedlist_iteratorhasnext(iter);
}
int sortedlist_iteratorhasprev(iterator* iter){
	return linkedlist_iteratorhasprev(iter);
}
void* sortedlist_iteratornext(iterator* iter){
	return linkedlist_iteratornext(iter);
}
void* sortedlist_iteratorprev(iterator* iter){
	return linkedlist_iteratorprev(iter);
}
void sortedlist_freeiter(iterator* iter){
	linkedlist_freeiter(iter);
}
void sortedlist_free(sortedlist* list){
	linkedlist_freeiter(list->iter);
	linkedlist_free(list->list);
	free(list);
}