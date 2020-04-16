#include<stdio.h>
#include<stdlib.h>

#include "gc.h"

typedef struct Alloc Alloc;

typedef struct Alloc {
    Alloc* next;
    short marked;
	short layout;
} Alloc;

Alloc* gc_head = NULL;
int gc_n_alloc = 0;

void* alloc_to_ptr(Alloc *alloc) {
	char* byte_oriented = (char*) alloc;
	char* offset = byte_oriented + sizeof(Alloc);
	return (void*) offset;
}

Alloc* ptr_to_alloc(void* ptr) {
	char* byte_oriented = (char*) ptr;
	char* offset = byte_oriented - sizeof(Alloc);
	return (Alloc*) offset;
}

void* gc_alloc(size_t size, short layout) {
    Alloc* node = (Alloc*) calloc(1, sizeof(Alloc) + size);
    node->next = gc_head;
	node->marked = 0;
	node->layout = layout;
    gc_head = node;
	gc_n_alloc++;
    return alloc_to_ptr(node);
}

void gc_clear_marks() {
    for (Alloc* node = gc_head; node; node = node->next) {
        node->marked = 0;
    }
}

void gc_mark_reachable(void* ptr) {
	if (ptr) {
		ptr_to_alloc(ptr)->marked = 1;
	}
}

void gc_mark_from(Alloc *alloc, int depth) {
	alloc->marked = 1;
	void *ptr = alloc_to_ptr(alloc);
	for (short layout = alloc->layout; layout; layout >>= 1, ptr += sizeof(void*)) {
		void **p = (void**) ptr;
		if (*p) {
			Alloc *next = ptr_to_alloc(*p);
			if (!next->marked)
				gc_mark_from(next, depth + 1);
		}
	}
}

void gc_mark_all() {
	for (Alloc *node = gc_head; node; node = node->next) {
		if (node->marked) {
			gc_mark_from(node, 0);
		}
	}
}

void gc_sweep() {
	Alloc **ptr = &gc_head;
	while (*ptr) {
		if (!(*ptr)->marked) {
			Alloc* to_free = *ptr;
			*ptr = (*ptr)->next;
			free(to_free);
			gc_n_alloc--;
		} else {
			ptr = &((*ptr)->next);
		}
	}
}

void gc_mark_and_sweep() {
	puts("mark all");
	gc_mark_all();
	puts("sweep");
	gc_sweep();
}

void print_gc_stats() {
	printf("gc allocated objects: %d\n", gc_n_alloc);
}
