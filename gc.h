#define CONS 1
#define INT 2

typedef struct Value Value;

typedef struct {
    Value* car;
    Value* cdr;
} Cons;

typedef struct Value {
    int kind;
    int marked;
    Value* next;
    union {
        Cons cons;
        int i;
    } body;
} Value;


void print_gc_stats();
void gc_mark_and_sweep();
Value* gc_alloc();
void gc_clear_marks();
void gc_mark_reachable(Value* value);