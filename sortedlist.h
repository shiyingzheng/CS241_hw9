#include "linkedlist.h"
typedef struct{
	linkedlist* list;
	int (*cmp)(void* a, void* b);
	iterator* iter;
} sortedlist;
extern sortedlist* sortedlist_init(int datasize, int (*cmp)(void* a, void* b));
extern void sortedlist_add(sortedlist* list,void* data);
extern void* sortedlist_rm(sortedlist* list, void* data); 
extern iterator* sortedlist_iterator(sortedlist* list);
extern int sortedlist_iteratorhasnext(iterator* iter);
extern int sortedlist_iteratorhasprev(iterator* iter);
extern void* sortedlist_iteratornext(iterator* iter);
extern void* sortedlist_iteratorprev(iterator* iter);
extern void sortedlist_freeiter(iterator* iter);
extern void sortedlist_free(sortedlist* list);
extern void* sortedlist_get_min(sortedlist* list);
extern void* sortedlist_get_max(sortedlist* list);
extern void* sortedlist_rm_min(sortedlist* list);
extern void* sortedlist_rm_max(sortedlist* list);