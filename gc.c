#include<stdio.h>
#include<stdlib.h>

#include "gc.h"


Value* gc_head = 0;
int gc_n_alloc = 0;

Value* gc_alloc() {
    Value* node = (Value*) calloc(1, sizeof(Value));
    node->next = gc_head;
    gc_head = node;
	gc_n_alloc++;
    return node;
}

void gc_clear_marks() {
    for (Value* node = gc_head; node; node = node->next) {
        node->marked = 0;
    }
}

void gc_mark_reachable(Value* value) {
	if (value) value->marked = 1;
}

void gc_mark_from(Value *value) {
	gc_mark_reachable(value);
	if (value->kind != CONS) {
		return;
	}
	Value *car = value->body.cons.car;
	if (car && !car->marked) {
		gc_mark_from(car);
	}
	Value *cdr = value->body.cons.cdr;
	if (cdr && !cdr->marked) {
		gc_mark_from(cdr);
	}
}

void gc_mark_all() {
	for (Value *node = gc_head; node; node = node->next) {
		if (node->marked) {
			gc_mark_from(node);
		}
	}
}

void gc_sweep() {
	Value **ptr = &gc_head;
	while (*ptr) {
		if (!(*ptr)->marked) {
			Value* to_free = *ptr;
			*ptr = (*ptr)->next;
			free(to_free);
			gc_n_alloc--;
		} else {
			ptr = &((*ptr)->next);
		}
	}
}

void gc_mark_and_sweep() {
	gc_mark_all();
	gc_sweep();
}

void print_gc_stats() {
	printf("gc allocated objects: %d\n", gc_n_alloc);
}
