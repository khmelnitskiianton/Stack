#ifndef STACK_BASE_H
#define STACK_BASE_H

int       stackCtor         (Stack_t *stk);
int       StackDtor         (Stack_t *stk);
int       push              (Stack_t *stk, Elem_t value);
Elem_t    pop               (Stack_t *stk, Elem_t* ret_value);
int       printing_stack    (Stack* stk, const char* file, const size_t line, const char* pretty_function);

#endif