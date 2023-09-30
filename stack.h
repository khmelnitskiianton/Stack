#ifndef STACK_H
#define STACK_H

int       stackCtor         (Stack_t *stk);
int       StackDtor         (Stack_t *stk);
int       push              (Stack_t *stk, Elem_t value);
Elem_t    pop               (Stack_t *stk, Elem_t* ret_value);
Elem_t*   expansion         (Stack_t *stk);
Elem_t*   comprassion       (Stack_t *stk);
int       checkStack        (Stack_t *stk, const char* func);
int       output_error      (Stack_t *stk, const char* file, const size_t line, const char* pretty_function, int error_code);
int       mem_poison        (void* memptr, size_t num);
int       degree            (int a, int b);
size_t    size_data         (Stack* stk);
FILE*     file_write        (void);
int       file_close        (FILE *file_text);
int       rewrite_hash      (Stack* stk, const char* ptr);
Hash_t    hash_func         (const char * str, size_t size);
Hash_t    MYHashFAQ6        (const char * str, size_t size);
int       printing_stack    (Stack* stk, const char* file, const size_t line, const char* pretty_function);
Canary_t* ptr_r_canary_data (Stack* stk);


#endif