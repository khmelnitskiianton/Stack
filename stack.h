#ifndef STACK_H
#define STACK_H

int     stackCtor    (Stack_t *stk);
int     StackDtor    (Stack_t *stk);
int     push         (Stack_t *stk, Elem_t value);
Elem_t  pop          (Stack_t *stk);
Elem_t* expansion    (Stack_t *stk);
Elem_t* comprassion  (Stack_t *stk);
int     checkStack   (Stack_t *stk);
int     output_error (Stack_t *stk, const char* file, const size_t line, const char* pretty_function, int error_code);

#define STACK_DUMP(stk, error_code) output_error(stk, __FILE__, __LINE__, __PRETTY_FUNCTION__, error_code);                                                       

#endif