#include<stdio.h>
#include<stdlib.h>

#include "gc.h"

Value* make_cons(Value *car, Value *cdr) {
	Value *node = gc_alloc();
	node->kind = CONS;
	node->body.cons.car = car;
	node->body.cons.cdr = cdr;
	return node;
}

Value* make_int(int i) {
	Value *node = gc_alloc();
	node->kind = INT;
	node->body.i = i;
	return node;
}

#define MAX_DEPTH 20

void print_value_inner(Value *node, int depth);

void print_list(Value *node, int depth) {
	if (!node) {
		return;
	}
	if (depth > MAX_DEPTH) {
		printf("...");
		return;
	}
	if (node->kind == INT) {
		printf(" . %d", node->body.i);
	} else if (node->kind == CONS) {
		printf(" ");
		print_value_inner(node->body.cons.car, depth + 1);
		print_list(node->body.cons.cdr, depth + 1);
	} else {
		printf("invalid node");
	}
}

void print_value_inner(Value *node, int depth) {
	if (!node) {
		printf("nil");
	} else if (node->kind == INT) {
		printf("%d", node->body.i);
	} else if (node->kind == CONS) {
		printf("(");
		print_value_inner(node->body.cons.car, depth + 1);
		print_list(node->body.cons.cdr, depth + 1);
		printf(")");
	} else {
		printf("invalid node");
	}
}

void print_value(Value *node) {
	print_value_inner(node, 0);
}

void show_value(char *name, Value *value) {
	printf("%s: ", name);
	print_value(value);
	printf("\n");
}

int main(int argc, char **argv) {
	Value *i = make_int(123);
	Value *nil = 0;
	Value *dotted_pair = make_cons(make_int(33), make_int(44));
	Value *proper_list = make_cons(make_int(33), make_cons(make_int(44), 0));
	Value *inner_nil = make_cons(make_int(1), make_cons(0, make_cons(make_int(2), 0)));
	
	// Allocated but not referenced:
	make_cons(make_int(3), make_cons(0, make_cons(make_int(7), 0)));
	
	Value *loop = make_cons(make_int(999), 0);
	loop->body.cons.cdr = loop;
	
	show_value("i", i);
	show_value("nil", nil);
	show_value("dotted_pair", dotted_pair);
	show_value("proper_list", proper_list);
	show_value("inner_nil", inner_nil);
	show_value("loop", loop);
	
	puts("");
	print_gc_stats();
	
	gc_clear_marks();
	gc_mark_reachable(i);
	gc_mark_reachable(nil);
	gc_mark_reachable(dotted_pair);
	gc_mark_reachable(proper_list);
	gc_mark_reachable(inner_nil);
	gc_mark_reachable(loop);
	gc_mark_and_sweep();
	
	puts("");
	print_gc_stats();
	
	puts("");
	show_value("i", i);
	show_value("nil", nil);
	show_value("dotted_pair", dotted_pair);
	show_value("proper_list", proper_list);
	show_value("inner_nil", inner_nil);
	show_value("loop", loop);

	gc_clear_marks();
	gc_mark_reachable(i);
	gc_mark_reachable(nil);
	gc_mark_reachable(dotted_pair);
	gc_mark_reachable(proper_list);
	gc_mark_reachable(inner_nil);
	// Don't mark loop as reachable this time
	gc_mark_and_sweep();
	
	puts("");
	print_gc_stats();
	
	puts("done");
	
    return 0;
}