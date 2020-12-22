#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "defines.h"

//defining new types for ease of use
typedef struct MemoryRangeNode MRNode;

//this is a suggested linked list, feel free to modify/scrap this if you want.
struct MemoryRangeNode {
    byte_ptr offset;
    uint32_t length;
    int use;

    MRNode* prev;
    MRNode* next;
};

void add_node(MRNode* current, MRNode* unique, size_t size);
void* mm_best_fit_malloc (size_t size);
void* mm_first_fit_malloc (size_t size);
void* mm_malloc (size_t size);
void  mm_free (void* ptr);
void  mm_print_heap_status (FILE*);
void  mm_defragment();
