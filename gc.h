void print_gc_stats();
void gc_mark_and_sweep();
void* gc_alloc(size_t size, short layout);
void gc_clear_marks();
void gc_mark_reachable(void* value);