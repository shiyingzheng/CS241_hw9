/*
 * Shiying Zheng and Ben Stern hw9
 * A test on the sorted list struct
 */

#include "sortedlist.h"
int intcmp(void* a, void* b){
	return *(int*)a-*(int*)b;
}
int main(){
	int n=50;
	int d=10000000;
	int r;
	//tests the sorting by putting a bunch of random numbers into the list
	//and looking too see whether they're sorted
	//dividing by d is just so that you can easily read the numbers
	sortedlist* list=sortedlist_init(sizeof(int),&intcmp);
	for(int i=0;i<n;++i){
		r=rand();
		//r=n-i;
		//printf("%d ",r);
		sortedlist_add(list,&r);
	}
	printf("front is %d\n",*(int*)sortedlist_get_min(list));
	free(sortedlist_rm_max(list));
	free(sortedlist_rm_min(list));
	iterator* iter=sortedlist_iterator(list);
	while(sortedlist_iteratorhasnext(iter)){
		printf("%d ",*(int*)sortedlist_iteratornext(iter));
	}
	printf("\n");
	
	sortedlist_freeiter(iter);
	sortedlist_free(list);
}
