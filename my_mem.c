#include "my_mem.h"
#include <stdlib.h>
#include <stdio.h>

void my_mminit() {
	heap = (byte_ptr) malloc (HEAP_SIZE);
	if (heap == 0) {
	printf("ERROR during mm init malloc\n");
	exit(1);
	}
	mm_inited = 1;

	//allocate a new list node
	head = malloc(sizeof(MRNode));
	//set it to the len of entire memory
	head->length = HEAP_SIZE;
	//this range starts at the start of the heap
	head->offset = heap;

	//STUDENT CODE
	(head -> use) = 0;
	(head -> prev) = NULL;
	(head -> next) = NULL;
	total_memory = 0;
}

void* mm_malloc(size_t size) {
    if (!mm_inited) 
        my_mminit();
 
    #ifdef BESTFIT
        return mm_best_fit_malloc(size);
    #elif FIRSTFIT
        return mm_first_fit_malloc(size);
    #else
        printf("memory system policy undefined\n");
        exit(1);
    #endif
}

void* mm_best_fit_malloc (size_t size) {
	total_memory += size;

	MRNode* current = head;
	MRNode* smallest_space = NULL; 
	size_t smallest = (HEAP_SIZE + 4);

	while (current != NULL) {
		size_t current_length = (current -> length);

		if ((current -> use) == 0 && current_length < smallest && current_length >= size) {
			if (current_length == size) {
				(current -> use) = 1;
				return (current -> offset);
			}

			smallest_space = current;
			smallest = current_length;
		}

		current = (current -> next);
	}

	if (smallest_space != NULL) {
		MRNode* new_node = malloc(sizeof(MRNode));
		add_node(smallest_space, new_node, size);
		return (new_node -> offset);
	}

	return 0;
}

void* mm_first_fit_malloc (size_t size) {
	total_memory += size;
	MRNode* current = head;

	while (current != NULL) {
		if ((current -> length) >= size && (current -> use) == 0) {
			if ((current -> length) != size){ 
				MRNode* to_add = malloc(sizeof(MRNode));
				add_node(current, to_add, size);
				return (to_add -> offset);
			} else {
				(current -> use) = 1;
				return (current -> offset);
			}
		}

		current = (current -> next);
	}

	return 0;
}

void mm_free(void* ptr) {
	if (ptr == 0 || ptr > (heap + HEAP_SIZE) || ptr < heap) {
		printf("Segmentation fault.\n");
		exit(1);
	}

	MRNode* current = head;
	int found = 0;

	while (current != NULL && !found) {
		if ((current -> offset) == ptr && (current -> use) == 1) {
			total_memory = total_memory - (current -> length);

			change_current(current);

			found = 1;

		} else if (((current -> offset) == ptr && (current -> use) == 0) || (current -> offset) > ptr) {
			printf("Segmentation fault.\n");
			exit(1);
		}

		current = (current -> next);
	}

}

void mm_print_heap_status(FILE* fout) {
	MRNode* current = head;

	while((current -> next) != NULL) {
		fprintf(fout, "%lu%s ", (current -> length), ((current -> use) == 0? "F" : "A"));
		current = (current -> next);
	}

	fprintf(fout, "%lu%s\n", (current -> length), ((current -> use) == 0? "F" : "A"));
}

void  mm_defragment() {
	size_t total_F_Mem = HEAP_SIZE - total_memory;
	byte_ptr new_pos = heap + total_F_Mem;
	MRNode* start = malloc(sizeof (MRNode));

	(start -> offset) = heap;
	(start -> length) = total_F_Mem;
	(start -> prev) = NULL;
	(start -> next) = head;
	(start -> use) = 0;

	(head -> prev) = start;
	MRNode* before = start;
	MRNode* current = head;


	while(current != NULL) {
		if((current -> use) != 1) {
			MRNode* temp = current;
			((current -> prev) -> next) = (current -> next);

			if((current -> next) != NULL)
				((current -> next) -> prev) = (current -> prev);

			current = (current -> next);
		} else {
			(current -> offset) = new_pos;
			new_pos = new_pos + (current -> length);
			(current -> prev) = before;

			before = current;
			current = (current -> next);
		}
	}

	head = start;
}


//Creating linked list functionalities in functions is a good idea
//e.g.  void add_node(Node* head, Node* new_node)
//if you do, declare those on the my_mem.h header file and put their code here.
void add_node(MRNode* current, MRNode* unique, size_t size) {
	(unique -> offset) = (current -> offset) + (current -> length) - size;
	(unique -> length) = size;
	(current -> length) -= size;
	(unique -> use) = 1;

	if ((current -> next) != NULL)
		((current -> next) -> prev) = unique;

	(unique -> next) = (current -> next);
	(unique -> prev) = current;
	(current -> next) = unique;
}

//mm_free() helper method. Code was getting very congested.
void change_current(MRNode* current) {
	(current -> use) = 0;

	if ((current -> next) != NULL && ((current -> next) -> use) == 0) {
		MRNode* temp = (current -> next);
		(current -> length) = (current -> length) + ((current -> next) -> length); 
		(current -> next) = ((current -> next) -> next);

		if((current -> next) != NULL)
			((current -> next) -> prev) = current;
	}

	if ((current -> prev) != NULL && ((current -> prev) -> use) == 0) {
		MRNode* temp = current; 
		((current -> prev) -> length) = ((current -> prev) -> length) + (current -> length); 
		((current -> prev) -> next) = (current -> next);

		if((current -> next) != NULL)
			((current -> next) -> prev) = (current -> prev);

		current = (current -> prev);
	}
}
